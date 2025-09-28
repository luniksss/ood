#ifndef CCINNAMON_H
#define CCINNAMON_H
#include "CCondimentDecorator.h"

// Добавка из корицы
class CCinnamon : public CCondimentDecorator
{
public:
    explicit CCinnamon(IBeveragePtr && beverage)
        : CCondimentDecorator(std::move(beverage))
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 20;
    }

    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return "Cinnamon";
    }
};

#endif //CCINNAMON_H
