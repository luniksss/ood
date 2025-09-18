#ifndef DRAWINGCOMMANDHANDLER_H
#define DRAWINGCOMMANDHANDLER_H

#pragma once
#include "../shapes/Shape.h"
#include "../picture/Picture.h"

class DrawingCommandHandler
{
public:
    DrawingCommandHandler(
        std::ostream& out
      , gfx::ICanvas &canvas
      , shapes::Picture &picture
      )
          : m_out(out)
          , m_canvas(canvas)
          , m_picture(picture)
    {}

    void List() const;
    void AddShape(std::istream& params) const;
    void MoveShape(std::istream& params) const;
    void DeleteShape(std::istream& params) const;
    void DrawShape(std::istream& params) const;

    void ChangeColor(std::istream& params) const;
    void ChangeShape(std::istream& params) const;

    void DrawPicture() const;
    void MovePicture(std::istream& params) const;


private:
    std::ostream& m_out;
    gfx::ICanvas& m_canvas;
    shapes::Picture& m_picture;
};

#endif //DRAWINGCOMMANDHANDLER_H
