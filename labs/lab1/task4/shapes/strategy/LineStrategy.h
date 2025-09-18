#ifndef LINESTRATEGY_H
#define LINESTRATEGY_H

#include "IShapeStrategy.h"
#include <sstream>
#include <string>

namespace shapes
{
class LineStrategy : public IShapeStrategy
{
public:
    LineStrategy(
        const Point start,
        const Point end
        )
        : m_start(start), m_end(end)
    {
    }

    void Move(const double dx, const double dy) override
    {
        m_start.m_x += dx;
        m_start.m_y += dy;
        m_end.m_x += dx;
        m_end.m_y += dy;
    }

    [[nodiscard]] std::string ShowInfo() const override
    {
        std::ostringstream oss;
        oss << m_start.m_x << " " << m_start.m_y << " " << m_end.m_x << " " << m_end.m_y;
        return oss.str();
    }

    [[nodiscard]] std::string GetShapeName() const override
    {
        return m_name;
    }

    void Draw(gfx::ICanvas& canvas, const Color color) const override
    {
        canvas.SetColor(color);
        canvas.MoveTo(m_start.m_x, m_start.m_y);
        canvas.LineTo(m_end.m_x, m_end.m_y);
    }

private:
    static constexpr auto m_name = "line";
    Point m_start;
    Point m_end;
};
}

#endif //LINESTRATEGY_H
