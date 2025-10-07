#include <gtest/gtest.h>
#include <vector>
#include "./FileHandlerMock.h"
#include "../InputStream/MemoryInputStream.h"
#include "../InputStream/FileInputStream.h"
#include "../OutputStream/FileOutputStream.h"
#include "../OutputStream/MemoryOutputStream.h"

class MemoryStreamsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_testData = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    }

    std::vector<uint8_t> m_testData;
};

class FileStreamsTest : public FileHandlerMock
{
protected:
    void SetUp() override
    {
        FileHandlerMock::SetUp();
        CreateTestFile({0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
    }
};

class DecoratorStreamsTest : public FileHandlerMock
{
protected:
    void SetUp() override
    {
        FileHandlerMock::SetUp();
    }
};

TEST_F(MemoryStreamsTest, MemoryInputStreamReadsData)
{
    std::vector<uint8_t> data = {0x00, 0x01, 0x02, 0x03, 0x04};
    MemoryInputStream stream(std::move(data));

    EXPECT_FALSE(stream.IsEOF());
    EXPECT_EQ(stream.ReadByte(), 0x00);
    EXPECT_EQ(stream.ReadByte(), 0x01);

    uint8_t buffer[3];
    const std::streamsize read = stream.ReadBlock(buffer, 3);
    EXPECT_EQ(read, 3);
    EXPECT_EQ(buffer[0], 0x02);
    EXPECT_EQ(buffer[1], 0x03);
    EXPECT_EQ(buffer[2], 0x04);
}

TEST_F(MemoryStreamsTest, MemoryOutputStreamWritesData)
{
    MemoryOutputStream stream;

    stream.WriteByte(0xAA);
    stream.WriteByte(0xBB);

    constexpr uint8_t block[] = {0xCC, 0xDD, 0xEE};
    stream.WriteBlock(block, 3);
    stream.Close();

    const std::vector<uint8_t> output = stream.GetData();
    EXPECT_EQ(output.size(), 5);
    EXPECT_EQ(output[0], 0xAA);
    EXPECT_EQ(output[1], 0xBB);
    EXPECT_EQ(output[2], 0xCC);
    EXPECT_EQ(output[3], 0xDD);
    EXPECT_EQ(output[4], 0xEE);
}

TEST_F(MemoryStreamsTest, MemoryInputStreamThrowsOnEOF)
{
    std::vector<uint8_t> data = {0x11, 0x22};
    MemoryInputStream stream(std::move(data));

    EXPECT_EQ(stream.ReadByte(), 0x11);
    EXPECT_EQ(stream.ReadByte(), 0x22);
    EXPECT_THROW(stream.ReadByte(), std::ios_base::failure);
}

TEST_F(FileStreamsTest, FileInputStreamReadsBytes)
{
    FileInputStream stream(m_testFilename);

    EXPECT_FALSE(stream.IsEOF());
    EXPECT_EQ(stream.ReadByte(), 0x00);
    EXPECT_EQ(stream.ReadByte(), 0x01);
    EXPECT_EQ(stream.ReadByte(), 0x02);
}

TEST_F(FileStreamsTest, FileInputStreamReadsBlocks)
{
    FileInputStream stream(m_testFilename);

    uint8_t buffer[3];
    const std::streamsize read = stream.ReadBlock(buffer, 3);

    EXPECT_EQ(read, 3);
    EXPECT_EQ(buffer[0], 0x00);
    EXPECT_EQ(buffer[1], 0x01);
    EXPECT_EQ(buffer[2], 0x02);
}

TEST_F(FileStreamsTest, FileOutputStreamWritesBytes)
{
    {
        FileOutputStream stream(m_outputFilename);
        stream.WriteByte(0xAA);
        stream.WriteByte(0xBB);
        stream.WriteByte(0xCC);
        stream.Close();
    }

    const auto data = ReadFile(m_outputFilename);
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[0], 0xAA);
    EXPECT_EQ(data[1], 0xBB);
    EXPECT_EQ(data[2], 0xCC);
}

TEST_F(FileStreamsTest, FileOutputStreamWritesBlocks)
{
    const uint8_t testBlock[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    {
        FileOutputStream stream(m_outputFilename);
        stream.WriteBlock(testBlock, sizeof(testBlock));
        stream.Close();
    }

    const auto data = ReadFile(m_outputFilename);
    ASSERT_EQ(data.size(), 5);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(data[i], testBlock[i]);
    }
}

TEST_F(FileStreamsTest, FileInputStreamThrowsOnNonExistentFile)
{
    EXPECT_THROW(FileInputStream stream("Z:/randomFile.bin"), std::ios_base::failure);
}

TEST_F(FileStreamsTest, FileOutputStreamThrowsOnNonExistentFile)
{
    EXPECT_THROW(FileOutputStream stream("Z:/randomFile.bin"), std::ios_base::failure);
}

TEST_F(FileStreamsTest, ReadByteThrowsOnEmptyFile)
{
    const std::string filename = "randomFile.bin";
    std::ofstream ofs(filename, std::ios::binary);
    ofs.close();

    FileInputStream stream(filename);
    EXPECT_THROW(stream.ReadByte(), std::ios_base::failure);

    std::remove(filename.c_str());
}

TEST_F(FileStreamsTest, ReadBlockThrowsOnEmptyFile)
{
    const std::string filename = "randomFile.bin";
    std::ofstream ofs(filename, std::ios::binary);
    ofs.close();

    FileInputStream stream(filename);
    char buffer[10];
    EXPECT_THROW(stream.ReadBlock(buffer, sizeof(buffer)), std::ios_base::failure);

    std::remove(filename.c_str());
}

TEST_F(FileStreamsTest, FileOutputStreamThrowsOnWriteByte)
{
    FileOutputStream stream("testFile.bin");
    stream.WriteByte(0xAA);
    stream.Close();

    EXPECT_THROW(stream.WriteByte(0xBB), std::ios_base::failure);
}

TEST_F(FileStreamsTest, FileOutputStreamThrowsOnWriteBlock)
{
    FileOutputStream stream("testFile.bin");
    constexpr uint8_t data[] = {0x01, 0x02};
    stream.WriteBlock(data, sizeof(data));
    stream.Close();

    EXPECT_THROW(stream.WriteBlock(data, sizeof(data)), std::ios_base::failure);
}