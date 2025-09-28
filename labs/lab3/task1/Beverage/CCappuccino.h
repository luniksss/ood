#ifndef CCAPPUCCINO_H
#define CCAPPUCCINO_H
#include "CCoffee.h"

// Капуччино
class CCappuccino : public CCoffee
{
public:
    CCappuccino()
        :CCoffee("Cappuccino")
    {}

    [[nodiscard]] double GetCost() const override
    {
        return 80;
    }
};

#endif //CCAPPUCCINO_H
