#ifndef ENCRYPTOUTPUTSTREAM_H
#define ENCRYPTOUTPUTSTREAM_H
#pragma once
#include <random>
#include <algorithm>
#include <array>
#include <memory>
#include "./OutputStreamDecorator.h"

class EncryptOutputStream : public OutputStreamDecorator
{
public:
    EncryptOutputStream(std::unique_ptr<IOutputDataStream> stream, const uint32_t key)
        : OutputStreamDecorator(std::move(stream))
        , m_encryptionTable(GenerateEncryptionTable(key))
    {}

    void WriteByte(const uint8_t data) override
    {
        m_outputStream->WriteByte(m_encryptionTable[data]);
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        const auto* data = static_cast<const uint8_t*>(srcData);
        for (std::streamsize i = 0; i < size; ++i)
        {
            WriteByte(data[i]);
        }
    }

private:
    std::array<uint8_t, 256> m_encryptionTable;

    static std::array<uint8_t, 256> GenerateEncryptionTable(const uint32_t key)
    {
        std::array<uint8_t, 256> table{};
        for (int i = 0; i < 256; ++i)
        {
            table[i] = static_cast<uint8_t>(i);
        }

        std::mt19937 gen(key);
        std::ranges::shuffle(table, gen);

        return table;
    }
};

#endif //ENCRYPTOUTPUTSTREAM_H
