#ifndef CSYRUP_H
#define CSYRUP_H
#include "CCondimentDecorator.h"

enum class SyrupType
{
    Chocolate,
    Maple,
};

// Добавка "Сироп"
class CSyrup : public CCondimentDecorator
{
public:
    CSyrup(IBeveragePtr && beverage, const SyrupType syrupType)
        : CCondimentDecorator(std::move(beverage))
        , m_syrupType(syrupType)
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 15;
    }

    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return std::string(m_syrupType == SyrupType::Chocolate ? "Chocolate" : "Maple")
            + " syrup";
    }
private:
    SyrupType m_syrupType;
};

#endif //CSYRUP_H
