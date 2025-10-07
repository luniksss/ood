#ifndef MEMORYOUTPUTSTREAM_H
#define MEMORYOUTPUTSTREAM_H
#include "./IOutputDataStream.h"
#include <vector>

class MemoryOutputStream : public IOutputDataStream
{
public:
    void WriteByte(uint8_t data) override
    {
        m_data.push_back(data);
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        const uint8_t* data = static_cast<const uint8_t*>(srcData);
        m_data.insert(m_data.end(), data, data + size);
    }

    void Close() override {};

    std::vector<uint8_t> GetData() const
    {
        return m_data;
    }

private:
    std::vector<uint8_t> m_data;
};

#endif //MEMORYOUTPUTSTREAM_H
