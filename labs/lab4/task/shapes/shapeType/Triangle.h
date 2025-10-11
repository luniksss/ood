#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "../Point.h"
#include "../Shape.h"
#include "../../canvas/ICanvas.h"
#include <stdexcept>
#include <cmath>

namespace shapes
{
class Triangle: public Shape
{
public:
    Triangle(
        const uint8_t color,
        const Point vertex1,
        const Point vertex2,
        const Point vertex3
    ):  Shape(color),
        m_vertex1(vertex1),
        m_vertex2(vertex2),
        m_vertex3(vertex3)
    {
      //проверка, что это существующий треугольник по 3 сторонам
        const double sideLine1 = GetSideLength(m_vertex2, m_vertex1);
        const double sideLine2 = GetSideLength(m_vertex3, m_vertex2);
        const double sideLine3 = GetSideLength(m_vertex1, m_vertex3);

        if ((sideLine1 + sideLine2 <= sideLine3)
            || (sideLine2 + sideLine3 <= sideLine1)
            || (sideLine1 + sideLine3 <= sideLine2))
        {
            throw std::invalid_argument("Triangle is not valid");
        }
    };

    Point GetVertex1() const
    {
        return m_vertex1;
    }

    Point GetVertex2() const
    {
        return m_vertex2;
    }

    Point GetVertex3() const
    {
        return m_vertex3;
    }

    void Draw(ICanvas& canvas, const uint8_t color) const override
    {
        canvas.SetColor(color);
        canvas.DrawTriangle(m_vertex1, m_vertex2, m_vertex3);
    }

private:
    Point m_vertex1, m_vertex2, m_vertex3;

    static double GetSideLength(const Point& vertex1, const Point& vertex2)
    {
        return std::sqrt(std::pow(vertex1.m_x - vertex2.m_x, 2) + std::pow(vertex1.m_y - vertex2.m_y, 2));
    };
};
}

#endif //TRIANGLE_H
