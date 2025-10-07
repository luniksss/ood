#ifndef FILEHANDLERMOCK_H
#define FILEHANDLERMOCK_H
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <gtest/gtest.h>

class FileHandlerMock : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_testFilename = "testFile.bin";
        m_outputFilename = "testOutput.bin";
        m_restoredFilename = "testRestored.bin";
    }

    void RemoveTestFiles() const
    {
        std::vector files_to_remove =
        {
            m_testFilename,
            m_outputFilename,
            m_restoredFilename
        };

        for (const auto& filename : files_to_remove)
        {
            if (std::filesystem::exists(filename))
            {
                std::remove(filename.c_str());
            }
        }
    }

    void CreateTestFile(const std::vector<uint8_t>& data) const
    {
        std::ofstream file(m_testFilename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }

    static std::vector<uint8_t> ReadFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        return std::vector<uint8_t>(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }

    std::string m_testFilename;
    std::string m_outputFilename;
    std::string m_restoredFilename;
};

#endif //FILEHANDLERMOCK_H
