#ifndef CLATTE_H
#define CLATTE_H
#include "CCoffee.h"

// Латте
class CLatte : public CCoffee
{
public:
    explicit CLatte(const bool isDouble)
        :CCoffee("Latte")
        , m_isDouble(isDouble)
    {}

    [[nodiscard]] double GetCost() const override
    {
        return m_isDouble ? 130 : 90;
    }
private:
    bool m_isDouble;
};

#endif //CLATTE_H
