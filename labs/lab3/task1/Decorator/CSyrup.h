#ifndef CSYRUP_H
#define CSYRUP_H
#include "CCondimentDecorator.h"

enum class SyrupType
{
    Chocolate,	// Шоколадный
    Maple,		// Кленовый
};

// Добавка "Сироп"
class CSyrup : public CCondimentDecorator
{
public:
    CSyrup(IBeveragePtr && beverage, SyrupType syrupType)
        : CCondimentDecorator(std::move(beverage))
        , m_syrupType(syrupType)
    {}
protected:
    double GetCondimentCost()const override
    {
        return 15;
    }
    std::string GetCondimentDescription()const override
    {
        return std::string(m_syrupType == SyrupType::Chocolate ? "Chocolate" : "Maple")
            + " syrup";
    }
private:
    SyrupType m_syrupType;
};

#endif //CSYRUP_H
