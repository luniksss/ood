#ifndef CCOFFEE_H
#define CCOFFEE_H
#include "CBeverage.h"

// Кофе
class CCoffee : public CBeverage
{
public:
    explicit CCoffee(const std::string& description = "Coffee")
        :CBeverage(description)
    {}

    [[nodiscard]] double GetCost() const override
    {
        return 60;
    }
};
#endif //CCOFFEE_H
