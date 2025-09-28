#ifndef CTEA_H
#define CTEA_H
#include "CBeverage.h"

// Чай
class CTea : public CBeverage
{
public:
    explicit CTea(const std::string& description = "Tea")
        :CBeverage(description)
    {}

    [[nodiscard]] double GetCost() const override
    {
        return 30;
    }
};

#endif //CTEA_H
