#ifndef CCREAM_H
#define CCREAM_H
#include "CCondimentDecorator.h"

// Добавка из сливок
class CCream : public CCondimentDecorator
{
public:
    explicit CCream(IBeveragePtr && beverage)
        : CCondimentDecorator(std::move(beverage))
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 25;
    }

    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return "Cream";
    }
};

#endif //CCREAM_H
