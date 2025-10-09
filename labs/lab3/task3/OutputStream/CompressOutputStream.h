#ifndef COMPRESSOUTPUTSTREAM_H
#define COMPRESSOUTPUTSTREAM_H
#include "./IOutputDataStream.h"
#include <memory>

class CompressOutputStream : public IOutputDataStream
{
public:
    explicit CompressOutputStream(std::unique_ptr<IOutputDataStream> stream)
        : m_stream(std::move(stream))
    {
    }

    void WriteByte(uint8_t data) override
    {
        if (!m_hasData)
        {
            m_lastByte = data;
            m_count = 1;
            m_hasData = true;
            return;
        }

        if (data == m_lastByte && m_count < 255)
        {
            m_count++;
        }
        else
        {
            FlushSequence();
            m_lastByte = data;
            m_count = 1;
            m_hasData = true;
        }
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        const auto* data = static_cast<const uint8_t*>(srcData);
        for (std::streamsize i = 0; i < size; ++i)
        {
            WriteByte(data[i]);
        }
    }

    void Close() override
    {
        FlushSequence();
        m_stream->Close();
    }

private:
    void FlushSequence()
    {
        if (!m_hasData) return;
        if (m_count >= 3)
        {
            m_stream->WriteByte(0xFF);
            m_stream->WriteByte(static_cast<uint8_t>(m_count));
            m_stream->WriteByte(m_lastByte);
        }
        else
        {
            for (int i = 1; i <= m_count; ++i)
            {
                m_stream->WriteByte(m_lastByte);
            }
        }

        m_hasData = false;
        m_count = 0;
    }

    std::unique_ptr<IOutputDataStream> m_stream;
    uint8_t m_lastByte = 0;
    int m_count = 0;
    bool m_hasData = false;
};

#endif //COMPRESSOUTPUTSTREAM_H
