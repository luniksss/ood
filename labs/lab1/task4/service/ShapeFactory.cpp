#include "ShapeFactory.h"

#include <iostream>
#include <memory>

std::unique_ptr<shapes::IShapeStrategy> ShapeFactory::CreateShape(std::istream& line)
{
    std::string shapeName;
    line >> shapeName;

    if (shapeName == "circle")
    {
        return createCircle(line);
    }
    if (shapeName == "line")
    {
        return createLine(line);
    }
    if (shapeName == "rectangle")
    {
        return createRectangle(line);
    }
    if (shapeName == "text")
    {
        return createText(line);
    }
    if (shapeName == "triangle")
    {
        return createTriangle(line);
    }

    return nullptr;
}

std::unique_ptr<shapes::CircleStrategy> ShapeFactory::createCircle(std::istream&  iss)
{
    double centerX, centerY, radius;
    if (!(iss >> centerX >> centerY >> radius))
    {
        throw std::invalid_argument("Invalid circle parameters");
    }

    shapes::Point centerPoint(centerX, centerY);
    return std::make_unique<shapes::CircleStrategy>(centerPoint, radius);
}

std::unique_ptr<shapes::LineStrategy> ShapeFactory::createLine(std::istream& iss)
{
    double startX, startY, endX, endY;
    if (!(iss >> startX >> endX >> startY >> endY))
    {
        throw std::invalid_argument("Invalid line segment coordinates");
    }

    shapes::Point startPoint(startX, endX);
    shapes::Point endPoint(startY, endY);

    return std::make_unique<shapes::LineStrategy>(startPoint, endPoint);
}

std::unique_ptr<shapes::RectangleStrategy> ShapeFactory::createRectangle(std::istream& iss)
{
    double leftTopX, leftTopY, height, width;
    if (!(iss >> leftTopX >> leftTopY >> height >> width))
    {
        throw std::invalid_argument("Invalid rectangle coordinates");
    }

    shapes::Point leftTopPoint(leftTopX, leftTopY);
    return std::make_unique<shapes::RectangleStrategy>(leftTopPoint, width, height);
}

std::unique_ptr<shapes::TextStrategy> ShapeFactory::createText(std::istream& iss)
{
    double leftTopX, leftTopY, fontSize;
    std::string text;
    if (!(iss >> leftTopX >> leftTopY >> fontSize >> text))
    {
        throw std::invalid_argument("Invalid rectangle coordinates");
    }

    shapes::Point leftTopPoint(leftTopX, leftTopY);
    return std::make_unique<shapes::TextStrategy>(leftTopPoint, fontSize, std::move(text));
}

std::unique_ptr<shapes::TriangleStrategy> ShapeFactory::createTriangle(std::istream& iss)
{
    double vertex1X, vertex1Y, vertex2X, vertex2Y, vertex3X, vertex3Y;
    if (!(iss >> vertex1X >> vertex1Y >> vertex2X >> vertex2Y >> vertex3X >> vertex3Y))
    {
        throw std::invalid_argument("Invalid number of triangle coordinates");
    }

    shapes::Point vertex1(vertex1X, vertex1Y);
    shapes::Point vertex2(vertex2X, vertex2Y);
    shapes::Point vertex3(vertex3X, vertex3Y);

    return std::make_unique<shapes::TriangleStrategy>(vertex1, vertex2, vertex3);
}