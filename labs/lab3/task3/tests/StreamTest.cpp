#include <gtest/gtest.h>
#include <vector>
#include "./FileHandlerMock.h"
#include "../InputStream/MemoryInputStream.h"
#include "../InputStream/FileInputStream.h"
#include "../InputStream/DecryptInputStream.h"
#include "../InputStream/DecompressInputStream.h"
#include "../OutputStream/FileOutputStream.h"
#include "../OutputStream/MemoryOutputStream.h"
#include "../OutputStream/EncryptOutputStream.h"
#include "../OutputStream/CompressOutputStream.h"

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

TEST_F(DecoratorStreamsTest, EncryptionChangesData)
{
    CreateTestFile({0x00, 0x01, 0x02, 0x03, 0x04, 0x05});
    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(m_outputFilename);
        EncryptOutputStream encryptStream(std::move(outputStream), 12345);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                encryptStream.WriteBlock(buffer, read);
            }
        }
        encryptStream.Close();
    }

    auto original = ReadFile(m_testFilename);
    auto encrypted = ReadFile(m_outputFilename);
    EXPECT_NE(original, encrypted);
    EXPECT_FALSE(encrypted.empty());
}

TEST_F(DecoratorStreamsTest, DecryptionChangesData)
{
    std::vector<uint8_t> testData = {0x00, 0x01, 0x02, 0x03, 0x04};
    CreateTestFile(testData);
    std::string encryptedFile = "encrypted_temp.bin";

    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(encryptedFile);
        EncryptOutputStream encryptStream(std::move(outputStream), 12345);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                encryptStream.WriteBlock(buffer, read);
            }
        }
        encryptStream.Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(encryptedFile);
        FileOutputStream outputStream(m_outputFilename);
        DecryptInputStream decryptStream(std::move(inputStream), 12345);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!decryptStream.IsEOF())
        {
            std::streamsize read = decryptStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream.WriteBlock(buffer, read);
            }
        }
        outputStream.Close();
    }

    auto encrypted = ReadFile(encryptedFile);
    auto decrypted = ReadFile(m_outputFilename);
    EXPECT_NE(encrypted, decrypted);
    EXPECT_FALSE(decrypted.empty());
    std::remove(encryptedFile.c_str());
}

TEST_F(DecoratorStreamsTest, EncryptionDecryptionRoundTrip)
{
    std::vector<uint8_t> testData = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    CreateTestFile(testData);

    std::string encryptedFile = "encrypted_temp.bin";
    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(encryptedFile);
        EncryptOutputStream encryptStream(std::move(outputStream), 12345);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                encryptStream.WriteBlock(buffer, read);
            }
        }
        encryptStream.Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(encryptedFile);
        FileOutputStream outputStream(m_outputFilename);
        DecryptInputStream decryptStream(std::move(inputStream), 12345);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!decryptStream.IsEOF())
        {
            std::streamsize read = decryptStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream.WriteBlock(buffer, read);
            }
        }
        outputStream.Close();
    }

    auto original = ReadFile(m_testFilename);
    auto restored = ReadFile(m_outputFilename);

    EXPECT_EQ(original, restored);

    std::remove(encryptedFile.c_str());
}

TEST_F(DecoratorStreamsTest, MultipleEncryptionDecryptionRoundTrip)
{
    std::vector<uint8_t> testData = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    CreateTestFile(testData);

    constexpr uint32_t keys[] = {111, 222, 333};
    std::string currentFile = m_testFilename;
    for (auto key : keys)
    {
        std::string nextFile = "temp_enc_" + std::to_string(key) + ".bin";
        {
            FileInputStream inputStream(currentFile);
            auto outputStream = std::make_unique<FileOutputStream>(nextFile);
            EncryptOutputStream encryptStream(std::move(outputStream), key);

            constexpr size_t bufferSize = 1024;
            uint8_t buffer[bufferSize];
            while (!inputStream.IsEOF())
            {
                std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
                if (read > 0)
                {
                    encryptStream.WriteBlock(buffer, read);
                }
            }
            encryptStream.Close();
        }

        if (currentFile != m_testFilename)
        {
            std::remove(currentFile.c_str());
        }
        currentFile = nextFile;
    }

    for (auto it = std::rbegin(keys); it != std::rend(keys); ++it)
    {
        std::string nextFile = "temp_dec_" + std::to_string(*it) + ".bin";
        {
            auto inputStream = std::make_unique<FileInputStream>(currentFile);
            FileOutputStream outputStream(nextFile);
            DecryptInputStream decryptStream(std::move(inputStream), *it);

            constexpr size_t bufferSize = 1024;
            uint8_t buffer[bufferSize];
            while (!decryptStream.IsEOF())
            {
                std::streamsize read = decryptStream.ReadBlock(buffer, bufferSize);
                if (read > 0)
                {
                    outputStream.WriteBlock(buffer, read);
                }
            }
            outputStream.Close();
        }

        std::remove(currentFile.c_str());
        currentFile = nextFile;
    }

    auto original = ReadFile(m_testFilename);
    auto restored = ReadFile(currentFile);

    EXPECT_EQ(original, restored);

    std::remove(currentFile.c_str());
}

TEST_F(DecoratorStreamsTest, DifferentKeysProduceDifferentEncryption)
{
    std::vector<uint8_t> testData = {0x00, 0x01, 0x02, 0x03, 0x04};
    CreateTestFile(testData);

    std::string encryptedFile1 = "encrypted1.bin";
    std::string encryptedFile2 = "encrypted2.bin";

    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(encryptedFile1);
        EncryptOutputStream encryptStream(std::move(outputStream), 11111);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                encryptStream.WriteBlock(buffer, read);
            }
        }
        encryptStream.Close();
    }

    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(encryptedFile2);
        EncryptOutputStream encryptStream(std::move(outputStream), 22222);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                encryptStream.WriteBlock(buffer, read);
            }
        }
        encryptStream.Close();
    }

    auto encrypted1 = ReadFile(encryptedFile1);
    auto encrypted2 = ReadFile(encryptedFile2);

    EXPECT_NE(encrypted1, encrypted2);

    std::remove(encryptedFile1.c_str());
    std::remove(encryptedFile2.c_str());
}

TEST_F(DecoratorStreamsTest, CompressionChangesData)
{
    std::vector<uint8_t> testData(100, 0xAA);
    CreateTestFile(testData);

    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(m_outputFilename);
        CompressOutputStream compressStream(std::move(outputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                compressStream.WriteBlock(buffer, read);
            }
        }
        compressStream.Close();
    }

    auto original = ReadFile(m_testFilename);
    auto compressed = ReadFile(m_outputFilename);

    EXPECT_NE(original, compressed);
    EXPECT_FALSE(compressed.empty());
    EXPECT_LE(compressed.size(), original.size());
}

TEST_F(DecoratorStreamsTest, DecompressionChangesData)
{
    std::vector<uint8_t> testData(100, 0xAA);
    CreateTestFile(testData);
    std::string compressedFile = "compressed_temp.bin";

    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(compressedFile);
        CompressOutputStream compressStream(std::move(outputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                compressStream.WriteBlock(buffer, read);
            }
        }
        compressStream.Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(compressedFile);
        FileOutputStream outputStream(m_outputFilename);
        DecompressInputStream decompressStream(std::move(inputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!decompressStream.IsEOF())
        {
            std::streamsize read = decompressStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream.WriteBlock(buffer, read);
            }
        }
        outputStream.Close();
    }

    auto compressed = ReadFile(compressedFile);
    auto decompressed = ReadFile(m_outputFilename);

    EXPECT_NE(compressed, decompressed);
    EXPECT_FALSE(decompressed.empty());

    std::remove(compressedFile.c_str());
}

TEST_F(DecoratorStreamsTest, CompressionDecompressionRoundTrip)
{
    std::vector<uint8_t> testData(100, 0xAA);
    CreateTestFile(testData);
    std::string compressedFile = "compressed_temp.bin";

    {
        FileInputStream inputStream(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(compressedFile);
        CompressOutputStream compressStream(std::move(outputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream.IsEOF())
        {
            std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                compressStream.WriteBlock(buffer, read);
            }
        }
        compressStream.Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(compressedFile);
        FileOutputStream outputStream(m_outputFilename);
        DecompressInputStream decompressStream(std::move(inputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!decompressStream.IsEOF())
        {
            std::streamsize read = decompressStream.ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream.WriteBlock(buffer, read);
            }
        }
        outputStream.Close();
    }

    auto original = ReadFile(m_testFilename);
    auto restored = ReadFile(m_outputFilename);

    EXPECT_EQ(original, restored);

    std::remove(compressedFile.c_str());
}

TEST_F(DecoratorStreamsTest, MultipleCompressionDecompressionRoundTrip)
{
    std::vector<uint8_t> testData(100, 0xAA);
    CreateTestFile(testData);
    constexpr int compressionLevels = 3;

    std::string currentFile = m_testFilename;
    for (int i = 0; i < compressionLevels; ++i)
    {
        std::string nextFile = "temp_comp_" + std::to_string(i) + ".bin";
        {
            FileInputStream inputStream(currentFile);
            auto outputStream = std::make_unique<FileOutputStream>(nextFile);
            CompressOutputStream compressStream(std::move(outputStream));

            constexpr size_t bufferSize = 1024;
            uint8_t buffer[bufferSize];
            while (!inputStream.IsEOF())
            {
                std::streamsize read = inputStream.ReadBlock(buffer, bufferSize);
                if (read > 0)
                {
                    compressStream.WriteBlock(buffer, read);
                }
            }
            compressStream.Close();
        }

        if (currentFile != m_testFilename)
        {
            std::remove(currentFile.c_str());
        }
        currentFile = nextFile;
    }

    for (int i = 0; i < compressionLevels; ++i)
    {
        std::string nextFile = "temp_decomp_" + std::to_string(i) + ".bin";
        {
            auto inputStream = std::make_unique<FileInputStream>(currentFile);
            FileOutputStream outputStream(nextFile);
            DecompressInputStream decompressStream(std::move(inputStream));

            constexpr size_t bufferSize = 1024;
            uint8_t buffer[bufferSize];
            while (!decompressStream.IsEOF())
            {
                std::streamsize read = decompressStream.ReadBlock(buffer, bufferSize);
                if (read > 0)
                {
                    outputStream.WriteBlock(buffer, read);
                }
            }
            outputStream.Close();
        }

        std::remove(currentFile.c_str());
        currentFile = nextFile;
    }

    auto original = ReadFile(m_testFilename);
    auto restored = ReadFile(currentFile);

    EXPECT_EQ(original, restored);

    std::remove(currentFile.c_str());
}

TEST_F(DecoratorStreamsTest, ComplexTransformationPipeline)
{
    std::vector<uint8_t> testData(100, 0xAA);
    CreateTestFile(testData);
    constexpr uint32_t encryptionKey = 12345;

    std::string compressedFile = "compressed.bin";
    std::string encryptedFile = "encrypted.bin";
    std::string decryptedFile = "decrypted.bin";
    std::string finalFile = "final.bin";

    {
        auto inputStream = std::make_unique<FileInputStream>(m_testFilename);
        auto outputStream = std::make_unique<FileOutputStream>(compressedFile);
        auto compressStream = std::make_unique<CompressOutputStream>(std::move(outputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream->IsEOF())
        {
            std::streamsize read = inputStream->ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                compressStream->WriteBlock(buffer, read);
            }
        }
        compressStream->Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(compressedFile);
        auto outputStream = std::make_unique<FileOutputStream>(encryptedFile);
        auto encryptStream = std::make_unique<EncryptOutputStream>(std::move(outputStream), encryptionKey);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!inputStream->IsEOF())
        {
            std::streamsize read = inputStream->ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                encryptStream->WriteBlock(buffer, read);
            }
        }
        encryptStream->Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(encryptedFile);
        auto outputStream = std::make_unique<FileOutputStream>(decryptedFile);
        auto decryptStream = std::make_unique<DecryptInputStream>(std::move(inputStream), encryptionKey);

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!decryptStream->IsEOF())
        {
            std::streamsize read = decryptStream->ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream->WriteBlock(buffer, read);
            }
        }
        outputStream->Close();
    }

    {
        auto inputStream = std::make_unique<FileInputStream>(decryptedFile);
        auto outputStream = std::make_unique<FileOutputStream>(finalFile);
        auto decompressStream = std::make_unique<DecompressInputStream>(std::move(inputStream));

        constexpr size_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (!decompressStream->IsEOF())
        {
            std::streamsize read = decompressStream->ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream->WriteBlock(buffer, read);
            }
        }
        outputStream->Close();
    }

    auto original = ReadFile(m_testFilename);
    auto finalResult = ReadFile(finalFile);
    EXPECT_EQ(original, finalResult);

    std::remove(compressedFile.c_str());
    std::remove(encryptedFile.c_str());
    std::remove(decryptedFile.c_str());
    std::remove(finalFile.c_str());
}