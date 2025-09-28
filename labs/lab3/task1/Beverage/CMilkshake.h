#ifndef CMILKSHAKE_H
#define CMILKSHAKE_H
#include "CBeverage.h"

enum class eMilkshakeSize
{
    Small,
    Middle,
    Large,
};

// Молочный коктейль
class CMilkshake : public CBeverage
{
public:
    explicit CMilkshake(const eMilkshakeSize size = eMilkshakeSize::Large)
        :CBeverage(ConvertMilkshakeSize(size) + "Milkshake")
        , m_size(size)
    {}

    [[nodiscard]] double GetCost() const override
    {
        switch (m_size)
        {
            case eMilkshakeSize::Small:
                return 50;
            case eMilkshakeSize::Middle:
                return 60;
            case eMilkshakeSize::Large:
                return 80;
        }
    }
private:
    eMilkshakeSize m_size;

    std::string ConvertMilkshakeSize(eMilkshakeSize size) const
    {
        std::string textSize;
        switch (size)
        {
            case eMilkshakeSize::Small:
                textSize = "Small ";
            break;
            case eMilkshakeSize::Middle:
                textSize = "Middle ";
            break;
            case eMilkshakeSize::Large:
                textSize = "Large ";
            break;
        }
        return textSize;
    }
};


#endif //CMILKSHAKE_H
