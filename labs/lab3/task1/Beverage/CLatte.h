#ifndef CLATTE_H
#define CLATTE_H
#include "CCoffee.h"

// Латте
class CLatte : public CCoffee
{
public:
    CLatte()
        :CCoffee("Latte")
    {}

    [[nodiscard]] double GetCost() const override
    {
        return 90;
    }
};

#endif //CLATTE_H
