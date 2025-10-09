#ifndef FILEINPUTSTREAM_H
#define FILEINPUTSTREAM_H
#include "./IInputDataStream.h"
#include <string>
#include <fstream>

class FileInputStream : public IInputDataStream
{
public:
    explicit FileInputStream(const std::string& filename)
    {
        m_file.open(filename, std::ios::binary);
        if (!m_file.is_open())
        {
            throw std::ios_base::failure("Cannot open file: " + filename);
        }
    };

    bool IsEOF() const override
    {
        return m_file.eof();
    }

    uint8_t ReadByte() override
    {
        char c;
        m_file.get(c);
        if (m_file.fail())
        {
            std::cerr << "FileInputStream::ReadByte FAILED" << std::endl;
            throw std::ios_base::failure("Read failed");
        }
        return static_cast<uint8_t>(c);
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        if (m_file.eof())
        {
            std::cerr << "FileInputStream::ReadBlock: already at EOF" << std::endl;
            throw std::ios_base::failure("End of file reached");
        }

        m_file.read(static_cast<char*>(dstBuffer), size);
        std::streamsize read = m_file.gcount();

        if (read == 0 && m_file.eof())
        {
            std::cerr << "FileInputStream::ReadBlock: read 0 bytes at EOF" << std::endl;
            throw std::ios_base::failure("Read failed - end of file");
        }

        if (m_file.fail() && !m_file.eof())
        {
            std::cerr << "FileInputStream::ReadBlock FAILED" << std::endl;
            throw std::ios_base::failure("Read failed");
        }

        return read;
    }

    ~FileInputStream() override
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

private:
    std::ifstream m_file;
};
#endif //FILEINPUTSTREAM_H
