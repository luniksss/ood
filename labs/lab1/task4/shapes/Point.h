#ifndef POINT_H
#define POINT_H

namespace shapes
{
class Point
{
public:
    Point(double x = 0.0, double y = 0.0)
        : m_x(x)
        , m_y(y)
    {
    }

    double m_x;
    double m_y;
};
}

#endif //POINT_H
