#ifndef SHAPE_H
#define SHAPE_H
#include "../canvas/ICanvas.h"

namespace shapes
{
class Shape
{
public:
    explicit Shape(const uint32_t color)
      : m_color(color)
    {}

    [[nodiscard]] uint32_t GetColor() const
    {
        return m_color;
    }

    virtual void Draw(ICanvas& canvas) const = 0;

    void SetColor(const uint32_t color)
    {
        m_color = color;
    }

    virtual ~Shape() = default;
private:
    uint32_t m_color;
};
}

#endif //SHAPE_H
