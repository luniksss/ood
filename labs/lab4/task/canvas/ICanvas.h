#ifndef ICANVAS_H
#define ICANVAS_H
#include "../shapes/Point.h"
#include <memory>

class ICanvas
{
public:
    virtual ~ICanvas() = default;
    virtual void SetColor(uint32_t color) = 0;
    virtual void MoveTo(double x, double y) = 0;
    virtual void LineTo(double x, double y) = 0;
    virtual void DrawLine(shapes::Point from, shapes::Point to) = 0;
    virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
    virtual void DrawRectangle(shapes::Point topLeft, double width, double height) = 0;
    virtual void DrawTriangle(shapes::Point vertex1, shapes::Point vertex2, shapes::Point vertex3) = 0;
};

#endif //ICANVAS_H
