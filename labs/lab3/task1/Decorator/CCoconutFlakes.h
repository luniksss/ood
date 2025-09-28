#ifndef CCOCONUTFLAKES_H
#define CCOCONUTFLAKES_H
#include "CCondimentDecorator.h"

// Кокосовая стружка
class CCoconutFlakes : public CCondimentDecorator
{
public:
    CCoconutFlakes(IBeveragePtr && beverage, const unsigned mass)
        : CCondimentDecorator(std::move(beverage))
        , m_mass(mass)
    {}

protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        return 1.0 * m_mass;
    }
    [[nodiscard]] std::string GetCondimentDescription() const override
    {
        return "Coconut flakes " + std::to_string(m_mass) + "g";
    }
private:
    unsigned m_mass;
};


#endif //CCOCONUTFLAKES_H
