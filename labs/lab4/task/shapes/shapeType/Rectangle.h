#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "../Point.h"
#include "../Shape.h"
#include "../../canvas/ICanvas.h"
#include <stdexcept>

namespace shapes
{
class Rectangle: public Shape
{
public:
    Rectangle(
        const uint32_t color,
        const Point topLeftPoint,
        const double width,
        const double height
    ):  Shape(color),
        m_topLeftPoint(topLeftPoint),
        m_width(width),
        m_height(height)
    {
        if (height <= 0 || width <= 0)
        {
          throw std::invalid_argument("Invalid rectangle params");
        }
    };

    Point GetStart() const
    {
        return m_topLeftPoint;
    }

    double GetWidth() const
    {
        return m_width;
    }

    double GetHeight() const
    {
        return m_height;
    }

    void Draw(ICanvas& canvas) const override
    {
        canvas.SetColor(GetColor());
        canvas.DrawRectangle(m_topLeftPoint, m_width, m_height);
    }

private:
    Point m_topLeftPoint;
    double m_width, m_height;
};
}

#endif //RECTANGLE_H
