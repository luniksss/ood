#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "../shapes/strategy/IShapeStrategy.h"
#include "../shapes/strategy/CircleStrategy.h"
#include "../shapes/strategy/LineStrategy.h"
#include "../shapes/strategy/RectangleStrategy.h"
#include "../shapes/strategy/TriangleStrategy.h"
#include "../shapes/strategy/TextStrategy.h"
#include <memory>

class ShapeFactory
{
public:
    static std::unique_ptr<shapes::IShapeStrategy> CreateShape(std::istream& line);

private:
    static std::unique_ptr<shapes::CircleStrategy> createCircle(std::istream& iss);
    static std::unique_ptr<shapes::LineStrategy> createLine(std::istream& iss);
    static std::unique_ptr<shapes::RectangleStrategy> createRectangle(std::istream& iss);
    static std::unique_ptr<shapes::TextStrategy> createText(std::istream& iss);
    static std::unique_ptr<shapes::TriangleStrategy> createTriangle(std::istream& iss);
};

#endif //SHAPEFACTORY_H
