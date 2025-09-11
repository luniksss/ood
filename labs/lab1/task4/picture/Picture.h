#ifndef PICTURE_H
#define PICTURE_H

#include <string>
#include <vector>
#include "../shapes/Shape.h"

namespace shapes
{
class Picture
{
public:
    std::vector<std::string> List();
    void AddShape(const std::string& id, std::unique_ptr<Shape> shape);
    void MoveShape(const std::string& id, double dx, double dy);
    void DeleteShape(const std::string &id);
    void DrawShape(const std::string& id, gfx::ICanvas &canvas);

    void ChangeColor(const std::string& id, Color color);
    void ChangeShape(const std::string &id, std::unique_ptr<IShapeStrategy> newShapeStrategy);

    void DrawPicture(gfx::ICanvas &canvas);
    void MovePicture(double dx, double dy);

    std::vector<std::string> GetShapeIds() const
    {
        return m_shapesId;
    }

    std::map<std::string, std::unique_ptr<Shape>> GetShapes()
    {
        return std::move(m_shapeList);
    }
private:
    std::vector<std::string> m_shapesId = {};
    std::map<std::string, std::unique_ptr<Shape>> m_shapeList = {};
};
}

#endif //PICTURE_H
