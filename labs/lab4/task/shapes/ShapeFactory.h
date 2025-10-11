#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H
#include "./IShapeFactory.h"
#include "./shapeType/Ellipse.h"
#include "./shapeType/Line.h"
#include "./shapeType/Rectangle.h"
#include "./shapeType/RegularPolygon.h"
#include "./shapeType/Triangle.h"
#include <iostream>
#include <memory>

class ShapeFactory : public IShapeFactory
{
public:
    std::unique_ptr<shapes::Shape> CreateShape(std::string description)
    {
        std::istringstream line(description);
        std::string color, shapeName;
        line >> shapeName >> color;
        uint8_t parsedColor = ParseColor(color);

        if (shapeName == "ellipse")
        {
            return CreateEllipse(parsedColor, line);
        }
        if (shapeName == "line")
        {
            return CreateLine(parsedColor, line);
        }
        if (shapeName == "rectangle")
        {
            return CreateRectangle(parsedColor, line);
        }
        if (shapeName == "polygon")
        {
            return CreateRegularPolygon(parsedColor, line);
        }
        if (shapeName == "triangle")
        {
            return CreateTriangle(parsedColor, line);
        }

        return nullptr;
    }

private:
    uint8_t ParseColor(std::string color)
    {
        try
        {
            if (!color.empty() && color[0] == '#')
            {
                color = color.substr(1);
            }

            uint8_t parsedColor = std::stoul(color, nullptr, 16);
            return parsedColor;
        }
        catch (std::exception const& e)
        {
            throw std::invalid_argument("Invalid color");
        }
    }

std::unique_ptr<shapes::Ellipse> CreateEllipse(uint8_t color, std::istream& iss)
{
    double centerX, centerY, radiusX, radiusY;
    if (!(iss >> centerX >> centerY >> radiusX >> radiusY))
    {
        throw std::invalid_argument("Invalid circle parameters");
    }

    shapes::Point centerPoint(centerX, centerY);
    return std::make_unique<shapes::Ellipse>(color, centerPoint, radiusX, radiusY);
}

std::unique_ptr<shapes::Line> CreateLine(uint8_t color, std::istream& iss)
{
    double startX, startY, endX, endY;
    if (!(iss >> startX >> endX >> startY >> endY))
    {
        throw std::invalid_argument("Invalid line segment coordinates");
    }

    shapes::Point startPoint(startX, endX);
    shapes::Point endPoint(startY, endY);

    return std::make_unique<shapes::Line>(color, startPoint, endPoint);
}

std::unique_ptr<shapes::Rectangle> CreateRectangle(uint8_t color, std::istream& iss)
{
    double leftTopX, leftTopY, height, width;
    if (!(iss >> leftTopX >> leftTopY >> height >> width))
    {
        throw std::invalid_argument("Invalid rectangle coordinates");
    }

    shapes::Point leftTopPoint(leftTopX, leftTopY);
    return std::make_unique<shapes::Rectangle>(color, leftTopPoint, width, height);
}

std::unique_ptr<shapes::RegularPolygon> CreateRegularPolygon(uint8_t color, std::istream& iss)
{
    int vertexCount;
    double centerX, centerY, radius;
    if (!(iss >> centerX >> centerY >> vertexCount >> radius))
    {
        throw std::invalid_argument("Invalid parameters for regular polygon");
    }
    if (radius < 0)
    {
        throw std::invalid_argument("Radius must be non-negative for regular polygon");
    }

    return std::make_unique<shapes::RegularPolygon>(color, shapes::Point(centerX, centerY), vertexCount, radius);
}

std::unique_ptr<shapes::Triangle> CreateTriangle(uint8_t color, std::istream& iss)
{
    double vertex1X, vertex1Y, vertex2X, vertex2Y, vertex3X, vertex3Y;
    if (!(iss >> vertex1X >> vertex1Y >> vertex2X >> vertex2Y >> vertex3X >> vertex3Y))
    {
        throw std::invalid_argument("Invalid number of triangle coordinates");
    }

    shapes::Point vertex1(vertex1X, vertex1Y);
    shapes::Point vertex2(vertex2X, vertex2Y);
    shapes::Point vertex3(vertex3X, vertex3Y);

    return std::make_unique<shapes::Triangle>(color, vertex1, vertex2, vertex3);
}
};
#endif //SHAPEFACTORY_H
