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

    [[nodiscard]] bool IsEOF() const override
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

    std::streamsize ReadBlock(void* dstBuffer, const std::streamsize size) override
    {
        auto* buffer = static_cast<uint8_t*>(dstBuffer);
        std::streamsize bytesRead = 0;

        while (bytesRead < size && !IsEOF())
        {
            buffer[bytesRead++] = ReadByte();
        }

        return bytesRead;
    }

private:
    std::vector<uint8_t> m_data;
    size_t m_position = 0;
};

#endif //MEMORYINPUTSTREAM_H
