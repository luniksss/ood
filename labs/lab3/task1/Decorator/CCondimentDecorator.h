#ifndef CCONDIMENTDECORATOR_H
#define CCONDIMENTDECORATOR_H
#include "../Beverage/IBeverage.h"

// Базовый декоратор "Добавка к напитку". Также является напитком
class CCondimentDecorator : public IBeverage
{
public:
    [[nodiscard]] std::string GetDescription() const override
    {
        // Описание декорированного напитка добавляется к описанию оборачиваемого напитка
        return m_beverage->GetDescription() + ", " + GetCondimentDescription();
    }

    [[nodiscard]] double GetCost() const override
    {
        // Стоимость складывается из стоимости добавки и стоимости декорируемого напитка
        return m_beverage->GetCost() + GetCondimentCost();
    }

    [[nodiscard]] virtual std::string GetCondimentDescription() const = 0;
    [[nodiscard]] virtual double GetCondimentCost() const = 0;
protected:
    explicit CCondimentDecorator(IBeveragePtr && beverage)
        : m_beverage(std::move(beverage))
    {
    }
private:
    IBeveragePtr m_beverage;
};

#endif //CCONDIMENTDECORATOR_H
