#ifndef CCAPPUCCINO_H
#define CCAPPUCCINO_H
#include "CCoffee.h"

// Капучино
class CCappuccino : public CCoffee
{
public:
    explicit CCappuccino(const bool isDouble)
        :CCoffee("Cappuccino")
        , m_isDouble(isDouble)
    {}

    [[nodiscard]] double GetCost() const override
    {
        return m_isDouble ? 120 : 80;
    }
private:
    bool m_isDouble = false;
};

#endif //CCAPPUCCINO_H
