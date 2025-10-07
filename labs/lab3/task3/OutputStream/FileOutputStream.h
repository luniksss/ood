#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H
#include "./IOutputDataStream.h"
#include <string>
#include <fstream>

class FileOutputStream : public IOutputDataStream
{
public:
    explicit FileOutputStream(const std::string& filename)
    {
        m_file.open(filename, std::ios::binary);
        if (!m_file.is_open())
        {
            throw std::ios_base::failure("Cannot open file: " + filename);
        }
    }

    void WriteByte(uint8_t data) override
    {
        std::cout << "FileOutputStream::WriteByte: 0x" << std::hex << (int)data << std::dec << std::endl;
        m_file.put(static_cast<char>(data));
        m_file.flush(); // Принудительно сбрасываем буфер
        if (m_file.fail())
        {
            std::cerr << "FileOutputStream::WriteByte FAILED" << std::endl;
            throw std::ios_base::failure("Write failed");
        }
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        m_file.write(static_cast<const char*>(srcData), size);
        if (m_file.fail())
        {
            throw std::ios_base::failure("Write failed");
        }
    }

    void Close() override
    {
        std::cout << "FileOutputStream::Close called" << std::endl;
        if (m_file.is_open())
        {
            m_file.flush();
            m_file.close();
            std::cout << "FileOutputStream closed successfully" << std::endl;
        }
    }

private:
    std::ofstream m_file;
};

#endif //FILEOUTPUTSTREAM_H
