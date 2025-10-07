#ifndef MEMORYINPUTSTREAM_H
#define MEMORYINPUTSTREAM_H
#include "./IInputDataStream.h"
#include <vector>
#include <algorithm>

class MemoryInputStream : public IInputDataStream
{
public:
    explicit MemoryInputStream(std::vector<uint8_t> data)
        : m_data(std::move(data))
    {
    }

    bool IsEOF() const override
    {
        return m_position >= m_data.size();
    }

    uint8_t ReadByte() override
    {
        if (IsEOF())
        {
            throw std::ios_base::failure("Read beyond EOF");
        }
        return m_data[m_position++];
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        std::streamsize bytesToRead = std::min(size, static_cast<std::streamsize>(m_data.size() - m_position));
        if (bytesToRead > 0)
        {
            std::copy(m_data.begin() + m_position, m_data.begin() + m_position + bytesToRead, static_cast<uint8_t*>(dstBuffer));
            m_position += bytesToRead;
        }
        return bytesToRead;
    }

private:
    std::vector<uint8_t> m_data;
    size_t m_position = 0;
};

#endif //MEMORYINPUTSTREAM_H
