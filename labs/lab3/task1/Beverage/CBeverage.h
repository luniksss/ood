#ifndef CBEVERAGE_H
#define CBEVERAGE_H
#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage
{
public:
    CBeverage(const std::string & description)
        :m_description(description)
    {}

    std::string GetDescription()const override final
    {
        return m_description;
    }
private:
    std::string m_description;
};

#endif //CBEVERAGE_H
