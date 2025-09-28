#ifndef CBEVERAGE_H
#define CBEVERAGE_H
#include <utility>
#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage
{
public:
    explicit CBeverage(std::string description)
        :m_description(std::move(description))
    {}

    [[nodiscard]] std::string GetDescription()const override
    {
        return m_description;
    }
private:
    std::string m_description;
};

#endif //CBEVERAGE_H
