#ifndef INPUTSTREAMDECORATOR_H
#define INPUTSTREAMDECORATOR_H
#include "./IInputDataStream.h"
#include <memory>

class InputStreamDecorator : public IInputDataStream
{
public:
    explicit InputStreamDecorator(std::unique_ptr<IInputDataStream> inputStream)
        : m_inputStream(std::move(inputStream))
    {
    }

    [[nodiscard]] bool IsEOF() const override
    {
        return m_inputStream->IsEOF();
    }

    uint8_t ReadByte() override
    {
        return m_inputStream->ReadByte();
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        return m_inputStream->ReadBlock(dstBuffer, size);
    }

protected:
    std::unique_ptr<IInputDataStream> m_inputStream;
};

#endif //INPUTSTREAMDECORATOR_H
