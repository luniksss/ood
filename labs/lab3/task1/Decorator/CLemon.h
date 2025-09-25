#ifndef CLEMON_H
#define CLEMON_H
#include "CCondimentDecorator.h"

// Лимонная добавка
class CLemon : public CCondimentDecorator
{
public:
    CLemon(IBeveragePtr && beverage, unsigned quantity = 1)
        : CCondimentDecorator(std::move(beverage))
        , m_quantity(quantity)
    {}
protected:
    double GetCondimentCost()const override
    {
        return 10.0 * m_quantity;
    }
    std::string GetCondimentDescription()const override
    {
        return "Lemon x " + std::to_string(m_quantity);
    }
private:
    unsigned m_quantity;
};

#endif //CLEMON_H
