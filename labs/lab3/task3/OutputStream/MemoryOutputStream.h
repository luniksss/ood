#ifndef MEMORYOUTPUTSTREAM_H
#define MEMORYOUTPUTSTREAM_H
#include "./IOutputDataStream.h"
#include <vector>

class MemoryOutputStream : public IOutputDataStream
{
public:
    void WriteByte(const uint8_t data) override
    {
        if (m_closed) throw std::logic_error("Stream closed");
        m_data.push_back(data);
    }

    void WriteBlock(const void* srcData, const std::streamsize size) override
    {
        if (m_closed) throw std::logic_error("Stream closed");
        const auto* data = static_cast<const uint8_t*>(srcData);
        m_data.insert(m_data.end(), data, data + size);
    }

    void Close() override
    {
        m_closed = true;
    }

    [[nodiscard]] std::vector<uint8_t> GetData() const
    {
        return m_data;
    }

private:
    std::vector<uint8_t> m_data;
    bool m_closed = false;
};

#endif //MEMORYOUTPUTSTREAM_H
