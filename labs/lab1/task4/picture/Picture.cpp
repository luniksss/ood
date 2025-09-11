#include "Picture.h"
#include <stdexcept>

using namespace shapes;
using namespace gfx;

std::vector<std::string> Picture::List()
{
    std::vector<std::string> result;
    int counter = 1;
    for (auto& shape : m_shapeList)
    {
        result.push_back
        (
            std::to_string(counter)
            + " "
            + shape.second->GetName()
            + " "
            + shape.second->GetColor()
            + " "
            + shape.second->GetId()
            + " "
            + shape.second->GetShapeInfo()
        );
        counter++;
    }

    return result;
}

void Picture::AddShape(const std::string& id, std::unique_ptr<Shape> shape)
{
    if (m_shapeList.contains(id))
    {
        throw std::invalid_argument("shape with this id already exist");
    }

    m_shapeList.emplace(id, std::move(shape));
    m_shapesId.push_back(id);
}

void Picture::MoveShape(const std::string& id, double x, double y)
{
    if (!m_shapeList.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    auto shape = m_shapeList.find(id);
    shape->second->Move(x, y);
}

void Picture::DeleteShape(const std::string& id)
{
    if (!m_shapeList.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    m_shapeList.erase(id);
    auto it = std::remove(m_shapesId.begin(), m_shapesId.end(), id);
    m_shapesId.erase(it, m_shapesId.end());
}

void Picture::DrawShape(const std::string& id, gfx::ICanvas &canvas)
{
    if (!m_shapeList.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    auto shape = m_shapeList.find(id);
    shape->second->Draw(canvas);
}

void Picture::ChangeColor(const std::string& id, Color color)
{
    if (!m_shapeList.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    auto shape = m_shapeList.find(id);
    shape->second->SetColor(color);
}

void Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeStrategy> newShapeStrategy)
{
    if (!m_shapeList.contains(id))
    {
        throw std::invalid_argument("Shape with given Id not exists");
    }

    auto shape = m_shapeList.find(id);
    shape->second->SetShapeStrategy(std::move(newShapeStrategy));
}

void Picture::DrawPicture(gfx::ICanvas &canvas)
{
    for (auto& shape : m_shapeList)
    {
        shape.second->Draw(canvas);
    }
};

void Picture::MovePicture(double x, double y)
{
    for (auto& shape : m_shapeList)
    {
        shape.second->Move(x, y);
    }
};