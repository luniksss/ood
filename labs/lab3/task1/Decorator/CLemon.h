#ifndef CLEMON_H
#define CLEMON_H
#include "CCondimentDecorator.h"

// Лимонная добавка
class CLemon : public CCondimentDecorator
{
public:
    explicit CLemon(IBeveragePtr && beverage, const unsigned quantity = 1)
        : CCondimentDecorator(std::move(beverage))
        , m_quantity(quantity)
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 10.0 * m_quantity;
    }

    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return "Lemon x " + std::to_string(m_quantity);
    }
private:
    unsigned m_quantity;
};

#endif //CLEMON_H
