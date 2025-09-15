#ifndef ICANVAS_H
#define ICANVAS_H
#include <regex>
#include "../shapes/Point.h"

namespace gfx
{
  using Color = uint32_t;
  class ICanvas
  {
  public:
    virtual ~ICanvas() = default;
    virtual void SetColor(Color color) = 0;
    virtual void MoveTo(double x, double y) = 0;
    virtual void LineTo(double x, double y) = 0;
    virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
    virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;
    virtual void DrawRectangle(shapes::Point topLeft, double width, double height) = 0;
    virtual void FillTriangle(shapes::Point vertex1, shapes::Point vertex2, shapes::Point vertex3) = 0;
    virtual void Display() = 0;
  };
}

#endif //ICANVAS_H
