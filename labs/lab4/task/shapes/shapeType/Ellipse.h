#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <stdexcept>
#include "../Shape.h"
#include "../Point.h"
#include "../../canvas/ICanvas.h"

namespace shapes
{
class Ellipse: public Shape
{
public:
    Ellipse(
        const uint32_t color,
        const Point center,
        const double radiusX,
        const double radiusY
    ):  Shape(color),
        m_centerPoint(center),
        m_radiusX(radiusX),
        m_radiusY(radiusY)
    {
        if (radiusX <= 0 || radiusY <= 0)
        {
            throw std::invalid_argument("Invalid radius");
        }
    };

    Point GetCenter() const
    {
      return m_centerPoint;
    }

    double GetRadiusX() const
    {
      return m_radiusX;
    }

    double GetRadiusY() const
    {
      return m_radiusY;
    }

    void Draw(ICanvas& canvas) const override
    {
        canvas.SetColor(GetColor());
        canvas.DrawEllipse(m_centerPoint.m_x, m_centerPoint.m_y, m_radiusX, m_radiusY);
    }
private:
    Point m_centerPoint;
    double m_radiusX;
    double m_radiusY;
};
}

#endif //ELLIPSE_H
