#ifndef CLIQUOR_H
#define CLIQUOR_H
#include "CCondimentDecorator.h"

enum class eLiquorType
{
    Chocolate,
    Nut,
};

// Добавка ликер
class CLiquor : public CCondimentDecorator
{
public:
    CLiquor(IBeveragePtr && beverage, const eLiquorType liquorType)
        : CCondimentDecorator(std::move(beverage))
        , m_liquorType(liquorType)
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 50;
    }

    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return std::string(m_liquorType == eLiquorType::Chocolate ? "Chocolate" : "Nut")
            + " liquor";
    }
private:
    eLiquorType m_liquorType;
};

#endif //CLIQUOR_H
