#ifndef OUTPUTSTREAMDECORATOR_H
#define OUTPUTSTREAMDECORATOR_H
#include "./IOutputDataStream.h"
#include <memory>

class OutputStreamDecorator : public IOutputDataStream
{
public:
    explicit OutputStreamDecorator(std::unique_ptr<IOutputDataStream> stream)
    : m_outputStream(std::move(stream))
    {
    }

    void WriteByte(const uint8_t data) override
    {
        m_outputStream->WriteByte(data);
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        m_outputStream->WriteBlock(srcData, size);
    }

    void Close() override
    {
        m_outputStream->Close();
    }

protected:
    std::unique_ptr<IOutputDataStream> m_outputStream;
};

#endif //OUTPUTSTREAMDECORATOR_H
