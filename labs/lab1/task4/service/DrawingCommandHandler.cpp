#include "DrawingCommandHandler.h"
#include "../shapes/strategy/IShapeStrategy.h"
#include "ShapeFactory.h"

void DrawingCommandHandler::List()
{
    for (auto const& shapeInfo : m_picture.List())
    {
        m_out << shapeInfo << std::endl;
    }
}

void DrawingCommandHandler::AddShape(std::istream& params)
{
    std::string id, color;
    params >> id >> color;
    if (id.empty())
    {
        throw std::invalid_argument("invalid command arguments, template: <id> <hex color> <shape type> <shape args>");
    }

    try
    {
        auto strategy = ShapeFactory::CreateShape(params);
        if (!color.empty() && color[0] == '#') {
            color = color.substr(1);
        }

        shapes::Color colorValue = std::stoul(color, nullptr, 16);
        auto shape = std::make_unique<shapes::Shape>(id, colorValue, std::move(strategy));
        m_picture.AddShape(id, std::move(shape));
    }
    catch (std::exception const& e)
    {
        m_out << e.what() << std::endl;
    }
}

void DrawingCommandHandler::MoveShape(std::istream& params)
{
    std::string id, dx, dy;
    params >> id >> dx >> dy;

    if (id.empty() || dx.empty() || dy.empty())
    {
        throw std::invalid_argument("invalid command arguments");
    }

    try
    {
        double doubleDx = stod(dx);
        double doubleDy = stod(dy);
        m_picture.MoveShape(id, doubleDx, doubleDy);
    }
    catch (std::exception const& e)
    {
        m_out << e.what() << std::endl;
    }
}

void DrawingCommandHandler::DeleteShape(std::istream& params)
{
    std::string id;
    params >> id;

    if (id.empty())
    {
        throw std::invalid_argument("invalid command arguments");
    }

    m_picture.DeleteShape(id);
}

void DrawingCommandHandler::DrawShape(std::istream& params)
{
    std::string id;
    params >> id;

    if (id.empty())
    {
        throw std::invalid_argument("invalid command arguments");
    }

    m_picture.DrawShape(id, m_canvas);
    m_canvas.Display();
}

void DrawingCommandHandler::ChangeColor(std::istream& params)
{
    std::string id, color;
    params >> id >> color;

    if (id.empty())
    {
        throw std::invalid_argument("invalid command arguments");
    }

    if (!color.empty() && color[0] == '#') {
        color = color.substr(1);
    }

    shapes::Color colorValue = std::stoul(color, nullptr, 16);
    m_picture.ChangeColor(id, colorValue);

}

void DrawingCommandHandler::ChangeShape(std::istream& params)
{
    std::string id;
    params >> id;
    if (id.empty())
    {
        throw std::invalid_argument("invalid command arguments");
    }

    try
    {
        auto newShapeStrategy = ShapeFactory::CreateShape(params);
        m_picture.ChangeShape(id, std::move(newShapeStrategy));
    }
    catch (std::exception const& e)
    {
        m_out << e.what() << std::endl;
    }
}

void DrawingCommandHandler::DrawPicture()
{
    m_picture.DrawPicture(m_canvas);
    m_canvas.Display();
}

void DrawingCommandHandler::MovePicture(std::istream& params)
{
    std::string dxStr, dyStr;
    params >> dxStr >> dyStr;

    if (dxStr.empty() || dyStr.empty())
    {
        throw std::invalid_argument("invalid command arguments");
    }

    try
    {
        double dx = stod(dxStr);
        double dy = stod(dyStr);
        m_picture.MovePicture(dx, dy);
    }
    catch (std::exception const& e)
    {
        m_out << e.what() << std::endl;
    }
}