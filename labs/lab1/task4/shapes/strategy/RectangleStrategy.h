#ifndef RECTANGLESTRATEGY_H
#define RECTANGLESTRATEGY_H

#include "IShapeStrategy.h"
#include <stdexcept>

namespace shapes
{
class RectangleStrategy: public IShapeStrategy
{
public:
  RectangleStrategy(Point topLeftPoint, double width, double height)
      : m_topLeftPoint(topLeftPoint)
      , m_width(width)
      , m_height(height)
  {
    if (height <= 0 || width <= 0)
    {
      throw std::invalid_argument("Invalid rectangle params");
    }
  };

  void Move(double dx, double dy) override
  {
    m_topLeftPoint.m_x += dx;
    m_topLeftPoint.m_y += dy;
  }

  std::string ShowInfo() const override
  {
    return std::to_string(m_topLeftPoint.m_x)
    + " " + std::to_string(m_topLeftPoint.m_y)
    + " " + std::to_string(m_width)
    + " " + std::to_string(m_height);
  }

  std::string GetShapeName() const override
  {
    return m_name;
  }

  void Draw(gfx::ICanvas& canvas, Color color) const override
  {
    canvas.SetColor(color);
    canvas.DrawRectangle(m_topLeftPoint, m_width, m_height);
  }

private:
  const std::string m_name = "rectangle";
  Point m_topLeftPoint;
  double m_width, m_height;
};
}

#endif //RECTANGLESTRATEGY_H
