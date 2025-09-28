#ifndef CTEA_H
#define CTEA_H
#include "CBeverage.h"

// Чай
class CTea : public CBeverage
{
public:
    CTea()
        :CBeverage("Tea")
    {}

    [[nodiscard]] double GetCost() const override
    {
        return 30;
    }
};

#endif //CTEA_H
