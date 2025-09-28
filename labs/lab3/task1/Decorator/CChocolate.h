#ifndef CCHOCOLATE_H
#define CCHOCOLATE_H
#include "CCondimentDecorator.h"

// Добавка шоколада (максимум - 5 долек)
class CChocolate : public CCondimentDecorator
{
public:
    CChocolate(
        IBeveragePtr && beverage,
        const unsigned quantity
    )
        : CCondimentDecorator(std::move(beverage))
        , m_quantity(quantity)
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 10 * m_quantity;
    }

    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return " chocolate x " + std::to_string(m_quantity);
    }
private:
    unsigned m_quantity;
};

#endif //CCHOCOLATE_H
