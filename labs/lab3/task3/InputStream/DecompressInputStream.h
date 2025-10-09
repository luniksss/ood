#ifndef DECOMPRESSINPUTSTREAM_H
#define DECOMPRESSINPUTSTREAM_H
#include "./IInputDataStream.h"
#include <memory>

class DecompressInputStream : public IInputDataStream
{
public:
    explicit DecompressInputStream(std::unique_ptr<IInputDataStream> stream)
        : m_stream(std::move(stream))
    {
    }

    [[nodiscard]] bool IsEOF() const override
    {
        return m_remainingCount == 0 && m_stream->IsEOF();
    }

    uint8_t ReadByte() override
    {
        if (IsEOF())
        {
            throw std::ios_base::failure("Attempt to read beyond EOF");
        }

        if (m_remainingCount > 0)
        {
            m_remainingCount--;
            return m_repeatedByte;
        }

        uint8_t byte = m_stream->ReadByte();
        if (byte == 0xFF && !m_stream->IsEOF())
        {
            try
            {
                uint8_t count = m_stream->ReadByte();
                m_repeatedByte = m_stream->ReadByte();
                m_remainingCount = count - 1;
                return m_repeatedByte;
            }
            catch (const std::ios_base::failure&)
            {
                return byte;
            }
        }

        return byte;
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        if (size == 0 || IsEOF())
        {
            return 0;
        }

        auto* buffer = static_cast<uint8_t*>(dstBuffer);
        std::streamsize bytesRead = 0;

        try
        {
            while (bytesRead < size && !IsEOF())
            {
                buffer[bytesRead++] = ReadByte();
            }
        }
        catch (const std::ios_base::failure& e)
        {
            if (bytesRead == 0) throw;
        }

        return bytesRead;
    }

private:
    std::unique_ptr<IInputDataStream> m_stream;
    uint8_t m_repeatedByte = 0;
    int m_remainingCount = 0;
};

#endif //DECOMPRESSINPUTSTREAM_H
