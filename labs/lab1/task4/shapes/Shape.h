#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <regex>
#include "./strategy/IShapeStrategy.h"

namespace shapes
{
class Shape
{
public:
    Shape(std::string id, const uint32_t color, std::unique_ptr<IShapeStrategy> shapeStrategy)
      : m_id(std::move(id))
      , m_color(color)
      , m_shapeStrategy(std::move(shapeStrategy))
    {}

    void Move(const double dx, const double dy) const
    {
        m_shapeStrategy->Move(dx, dy);
    }

    [[nodiscard]] std::string GetId() const
    {
        return m_id;
    }

    [[nodiscard]] std::string GetName() const
    {
        return m_shapeStrategy->GetShapeName();
    }

    [[nodiscard]] Color GetColor() const
    {
        return m_color;
    }

    [[nodiscard]] std::string GetShapeInfo() const
    {
        return m_shapeStrategy->ShowInfo();
    }

    void Draw(gfx::ICanvas& canvas) const
    {
        m_shapeStrategy->Draw(canvas, m_color);
    }

    void SetColor(const Color color)
    {
        m_color = color;
    }

    void SetShapeStrategy(std::unique_ptr<IShapeStrategy> newShapeStrategy)
    {
        m_shapeStrategy = std::move(newShapeStrategy);
    }

    ~Shape() = default;
private:
    std::string m_id;
    Color m_color;
    std::unique_ptr<IShapeStrategy> m_shapeStrategy;
};
}

#endif //SHAPE_H
