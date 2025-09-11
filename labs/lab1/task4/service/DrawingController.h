#ifndef DRAWINGCONTROLLER_H
#define DRAWINGCONTROLLER_H

#include <iostream>
#include "DrawingCommandHandler.h"
#include "../gfx/ICanvas.h"

class DrawingController
{
public:
    DrawingController(
      std::istream& in
      , DrawingCommandHandler& handler)
      : m_in(in)
      , m_shapeHandler(handler)
    {
        m_commands.emplace("List", [this](std::istream& params) { m_shapeHandler.List(); });
        m_commands.emplace("AddShape", [this](std::istream& params) { m_shapeHandler.AddShape(params); });
        m_commands.emplace("MoveShape", [this](std::istream& params) { m_shapeHandler.MoveShape(params); });
        m_commands.emplace("DeleteShape", [this](std::istream& params) { m_shapeHandler.DeleteShape(params); });
        m_commands.emplace("DrawShape", [this](std::istream& params) { m_shapeHandler.DrawShape(params); });

        m_commands.emplace("ChangeColor", [this](std::istream& params) { m_shapeHandler.ChangeColor(params); });
        m_commands.emplace("ChangeShape", [this](std::istream& params) { m_shapeHandler.ChangeShape(params); });

        m_commands.emplace("DrawPicture", [this](std::istream& params) { m_shapeHandler.DrawPicture(); });
        m_commands.emplace("MovePicture", [this](std::istream& params) { m_shapeHandler.MovePicture(params); });

    }

    int StartDrawing();

private:
    std::istream& m_in;
    DrawingCommandHandler& m_shapeHandler;

    using DrawingCommand = std::function<void(std::istream& params)>;
    std::map<std::string, DrawingCommand> m_commands;
};

#endif //DRAWINGCONTROLLER_H
