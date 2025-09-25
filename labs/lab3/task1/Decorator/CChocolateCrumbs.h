#ifndef CCHOCOLATECRUMBS_H
#define CCHOCOLATECRUMBS_H
#include "CCondimentDecorator.h"

// Шоколадная крошка
class CChocolateCrumbs : public CCondimentDecorator
{
public:
    CChocolateCrumbs(IBeveragePtr && beverage, unsigned mass)
        :CCondimentDecorator(std::move(beverage))
        , m_mass(mass)
    {
    }

    double GetCondimentCost()const override
    {
        return 2.0 * m_mass;
    }

    std::string GetCondimentDescription()const override
    {
        return "Chocolate crumbs " + std::to_string(m_mass) + "g";
    }
private:
    unsigned m_mass;
};

#endif //CCHOCOLATECRUMBS_H
