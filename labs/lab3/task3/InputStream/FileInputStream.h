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
        m_file.exceptions(std::ios::failbit | std::ios::badbit);
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
        std::cout << "FileInputStream::ReadByte: read 0x" << std::hex << static_cast<int>(static_cast<uint8_t>(c))
                  << " (" << (c >= 32 && c <= 126 ? c : '.') << ")" << std::dec
                  << ", eof: " << m_file.eof()
                  << ", fail: " << m_file.fail()
                  << ", bad: " << m_file.bad() << std::endl;

        if (m_file.fail())
        {
            std::cerr << "FileInputStream::ReadByte FAILED" << std::endl;
            throw std::ios_base::failure("Read failed");
        }
        return static_cast<uint8_t>(c);
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        std::cout << "FileInputStream::ReadBlock called, size: " << size << std::endl;

        m_file.read(static_cast<char*>(dstBuffer), size);
        std::streamsize read = m_file.gcount();

        std::cout << "FileInputStream::ReadBlock: read " << read << " bytes" << std::endl;
        std::cout << "File status - eof: " << m_file.eof()
                  << ", fail: " << m_file.fail()
                  << ", bad: " << m_file.bad() << std::endl;

        if (m_file.fail() && !m_file.eof())
        {
            std::cerr << "FileInputStream::ReadBlock FAILED" << std::endl;
            throw std::ios_base::failure("Read failed");
        }
        return read;
    }

private:
    std::ifstream m_file;
};
#endif //FILEINPUTSTREAM_H
