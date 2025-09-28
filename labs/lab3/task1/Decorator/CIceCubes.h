#ifndef CICECUBES_H
#define CICECUBES_H
#include "CCondimentDecorator.h"

enum class IceCubeType
{
    Dry,
    Water
};

// Добавка "Кубики льда". Определяется типом и количеством кубиков, что влияет на стоимость
// и описание
class CIceCubes : public CCondimentDecorator
{
public:
    CIceCubes(
        IBeveragePtr && beverage,
        const unsigned quantity,
        const IceCubeType type = IceCubeType::Water
    )
        : CCondimentDecorator(std::move(beverage))
        , m_quantity(quantity)
        , m_type(type)
    {}
protected:
    [[nodiscard]] double GetCondimentCost() const override
    {
        // Чем больше кубиков, тем больше стоимость.
        // Сухой лед стоит дороже
        return (m_type == IceCubeType::Dry ? 10 : 5) * m_quantity;
    }
    [[nodiscard]] std::string GetCondimentDescription()const override
    {
        return std::string(m_type == IceCubeType::Dry ? "Dry" : "Water")
            + " ice cubes x "
            + std::to_string(m_quantity);
    }
private:
    unsigned m_quantity;
    IceCubeType m_type;
};

#endif //CICECUBES_H
