#ifndef LINE_H
#define LINE_H
#include "../Point.h"
#include "../Shape.h"
#include "../../canvas/ICanvas.h"

namespace shapes
{
class Line: public Shape
{
public:
    Line(
        const uint32_t color,
        const Point start,
        const Point end
    ):  Shape(color),
        m_start(start),
        m_end(end)
    {
    }

    Point GetStart() const
    {
        return m_start;
    }

    Point GetEnd() const
    {
        return m_end;
    }

    void Draw(ICanvas& canvas) const override
    {
        canvas.SetColor(GetColor());
        canvas.MoveTo(m_start.m_x, m_start.m_y);
        canvas.LineTo(m_end.m_x, m_end.m_y);
    }

private:
    Point m_start;
    Point m_end;
};
}

#endif //LINE_H
