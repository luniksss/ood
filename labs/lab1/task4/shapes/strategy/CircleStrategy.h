#ifndef CIRCLESTRATEGY_H
#define CIRCLESTRATEGY_H

#include "IShapeStrategy.h"
#include <stdexcept>

namespace shapes
{
class CircleStrategy: public IShapeStrategy
{
public:
    CircleStrategy(const Point center, const double radius)
      : m_centerPoint(center),
      m_radius(radius)
    {
        if (radius <= 0)
        {
            throw std::invalid_argument("Invalid radius");
        }
    };

    void Move(const double dx, const double dy) override
    {
        m_centerPoint.m_x += dx;
        m_centerPoint.m_y += dy;
    }

    [[nodiscard]] std::string ShowInfo() const override
    {
        return std::to_string(m_centerPoint.m_x)
        + " " + std::to_string(m_centerPoint.m_y)
        + " " + std::to_string(m_radius);
    }

    [[nodiscard]] std::string GetShapeName() const override
    {
        return m_name;
    }

    void Draw(gfx::ICanvas& canvas, Color color) const override
    {
        canvas.SetColor(color);
        canvas.DrawEllipse(m_centerPoint.m_x, m_centerPoint.m_y, m_radius, m_radius);
    }
private:
    const std::string m_name = "circle";
    Point m_centerPoint;
    double m_radius;
};
}

#endif //CIRCLESTRATEGY_H
