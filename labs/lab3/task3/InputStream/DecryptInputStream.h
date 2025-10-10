#ifndef DECRYPTINPUTSTREAM_H
#define DECRYPTINPUTSTREAM_H
#pragma once
#include <random>
#include <algorithm>
#include <array>
#include <memory>
#include "./InputStreamDecorator.h"

class DecryptInputStream : public InputStreamDecorator
{
public:
    DecryptInputStream(std::unique_ptr<IInputDataStream> stream, const uint32_t key)
        : InputStreamDecorator(std::move(stream))
        , m_decryptionTable(GenerateDecryptionTable(key))
    {
    }

    uint8_t ReadByte() override
    {
        const uint8_t encrypted = m_inputStream->ReadByte();
        return m_decryptionTable[encrypted];
    }

    std::streamsize ReadBlock(void* dstBuffer, const std::streamsize size) override
    {
        auto* buffer = static_cast<uint8_t*>(dstBuffer);
        std::streamsize totalRead = 0;

        while (totalRead < size && !IsEOF())
        {
            try
            {
                buffer[totalRead] = ReadByte();
                totalRead++;
            }
            catch (const std::ios_base::failure& e)
            {
                if (IsEOF())
                {
                    break;
                }
                throw;
            }
        }

        return totalRead;
    }

private:
    std::array<uint8_t, 256> m_decryptionTable;

    static std::array<uint8_t, 256> GenerateDecryptionTable(const uint32_t key)
    {
        std::array<uint8_t, 256> encryptionTable{};
        for (int i = 0; i < 256; ++i)
        {
            encryptionTable[i] = static_cast<uint8_t>(i);
        }

        std::mt19937 gen(key);
        std::ranges::shuffle(encryptionTable, gen);

        std::array<uint8_t, 256> decryptionTable{};
        for (int i = 0; i < 256; ++i)
        {
            decryptionTable[encryptionTable[i]] = static_cast<uint8_t>(i);
        }

        return decryptionTable;
    }
};

#endif //DECRYPTINPUTSTREAM_H
