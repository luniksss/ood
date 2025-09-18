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
    [[nodiscard]] std::vector<std::string> List() const;
    void AddShape(const std::string& id, std::unique_ptr<Shape> shape);
    void MoveShape(const std::string& id, double dx, double dy) const;
    void DeleteShape(const std::string &id);
    void DrawShape(const std::string& id, gfx::ICanvas &canvas) const;

    void ChangeColor(const std::string& id, Color color) const;
    void ChangeShape(const std::string &id, std::unique_ptr<IShapeStrategy> newShapeStrategy) const;

    void DrawPicture(gfx::ICanvas &canvas) const;
    void MovePicture(double dx, double dy) const;

    [[nodiscard]] std::vector<std::string> GetShapeIds() const
    {
        return m_shapesId;
    }

    std::map<std::string, std::unique_ptr<Shape>>& GetShapes()
    {
        return m_shapeList;
    }
private:
    std::vector<std::string> m_shapesId = {};
    std::map<std::string, std::unique_ptr<Shape>> m_shapeList = {};
};
}

#endif //PICTURE_H
