#ifndef TEXTSTRATEGY_H
#define TEXTSTRATEGY_H

#include "IShapeStrategy.h"
#include <stdexcept>

namespace shapes
{
class TextStrategy: public IShapeStrategy
{
public:
    TextStrategy(const Point topLeftPoint, const double size, std::string&& text)
        : m_topLeftPoint(topLeftPoint)
        , m_size(size)
        , m_text(text)
    {
        if (size <= 0)
        {
            throw std::invalid_argument("Invalid text size");
        }
    };

    void Move(const double dx, const double dy) override
    {
        m_topLeftPoint.m_x += dx;
        m_topLeftPoint.m_y += dy;
    }

    [[nodiscard]] std::string ShowInfo() const override
    {
        return std::to_string(m_topLeftPoint.m_x)
        + " " + std::to_string(m_topLeftPoint.m_y)
        + " " + std::to_string(m_size)
        + " " + m_text;
    }

    [[nodiscard]] std::string GetShapeName() const override
    {
        return m_name;
    }

    void Draw(gfx::ICanvas& canvas, Color color) const override
    {
        canvas.SetColor(color);
        canvas.DrawText(m_topLeftPoint.m_x, m_topLeftPoint.m_y, m_size, m_text);
    }

private:
    const std::string m_name = "text";
    Point m_topLeftPoint;
    double m_size;
    std::string m_text;
};
}

#endif //TEXTSTRATEGY_H
