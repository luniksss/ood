#ifndef TRIANGLESTRATEGY_H
#define TRIANGLESTRATEGY_H

#include "IShapeStrategy.h"
#include <stdexcept>
#include <cmath>

namespace shapes
{
class TriangleStrategy: public IShapeStrategy
{
public:
    TriangleStrategy(const Point vertex1, const Point vertex2, const Point vertex3)
        : m_vertex1(vertex1)
        , m_vertex2(vertex2)
        , m_vertex3(vertex3)
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

    void Move(const double dx, const double dy) override
    {
        m_vertex1.m_x += dx;
        m_vertex1.m_y += dy;

        m_vertex2.m_x += dx;
        m_vertex2.m_y += dy;

        m_vertex3.m_x += dx;
        m_vertex3.m_y += dy;
    }

    [[nodiscard]] std::string ShowInfo() const override
    {
        return std::to_string(m_vertex1.m_x)
        + " " + std::to_string(m_vertex1.m_y)
        + " " + std::to_string(m_vertex2.m_x)
        + " " + std::to_string(m_vertex2.m_y)
        + " " + std::to_string(m_vertex3.m_x)
        + " " + std::to_string(m_vertex3.m_y);
    }

    [[nodiscard]] std::string GetShapeName() const override
    {
        return m_name;
    }

    void Draw(gfx::ICanvas& canvas, const Color color) const override
    {
        canvas.SetColor(color);
        canvas.DrawTriangle(m_vertex1, m_vertex2, m_vertex3);
    }

private:
    const std::string m_name = "triangle";
    Point m_vertex1, m_vertex2, m_vertex3;

    static double GetSideLength(const Point& vertex1, const Point& vertex2)
    {
        return std::sqrt(std::pow(vertex1.m_x - vertex2.m_x, 2) + std::pow(vertex1.m_y - vertex2.m_y, 2));
    };
};
}

#endif //TRIANGLESTRATEGY_H
