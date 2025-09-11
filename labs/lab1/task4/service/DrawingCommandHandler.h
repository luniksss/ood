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

    void List();
    void AddShape(std::istream& params);
    void MoveShape(std::istream& params);
    void DeleteShape(std::istream& params);
    void DrawShape(std::istream& params);

    void ChangeColor(std::istream& params);
    void ChangeShape(std::istream& params);

    void DrawPicture();
    void MovePicture(std::istream& params);


private:
    std::ostream& m_out;
    gfx::ICanvas& m_canvas;
    shapes::Picture& m_picture;
};

#endif //DRAWINGCOMMANDHANDLER_H
