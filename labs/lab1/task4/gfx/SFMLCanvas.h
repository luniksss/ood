#ifndef SFMLCANVAS_H
#define SFMLCANVAS_H
#include <SFML/Graphics.hpp>
#include "ICanvas.h"

using namespace shapes;

namespace gfx
{
class SFMLCanvas: public ICanvas
{
public:
  explicit SFMLCanvas(sf::RenderWindow& window) : m_window(window) {}

  void SetColor(const Color color) override
  {
    sf::Color result = uint32ToSFMLColor(color);
    m_color = result;
  }

  void MoveTo(double x, double y) override
  {
    m_currentPoint = { x, y };
  }

  void LineTo(double x, double y) override
  {
    shapes::Point destinationPoint = { x, y };
    drawLine(m_currentPoint, destinationPoint);
    m_currentPoint = destinationPoint;
  }

  void DrawEllipse(double cx, double cy, double rx, double ry) override
  {
    sf::CircleShape ellipse(1.f);
    ellipse.setPosition(cx - rx, cy - ry);
    ellipse.setScale(rx, ry);
    ellipse.setFillColor(m_color);
    ellipse.setOutlineThickness(2);
    m_window.draw(ellipse);
  }

  void DrawText(double left, double top, double fontSize, const std::string& text) override
  {
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded)
    {
      if (!font.loadFromFile("./font/Merriweather-Regular.ttf"))
      {
        return;
      }
      fontLoaded = true;
    }

    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(static_cast<unsigned int>(fontSize));
    sfText.setFillColor(m_color);
    sfText.setPosition(static_cast<float>(left), static_cast<float>(top));
    m_window.draw(sfText);
  }

  void DrawRectangle(shapes::Point topLeft, double width, double height) override
  {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    rectangle.setPosition(static_cast<float>(topLeft.m_x), static_cast<float>(topLeft.m_y));
    rectangle.setFillColor(m_color);
    rectangle.setOutlineThickness(2);

    m_window.draw(rectangle);
  }

  void FillTriangle(shapes::Point vertex1, shapes::Point vertex2, shapes::Point vertex3) override
  {
    sf::VertexArray triangle(sf::Triangles, 3);

    triangle[0].position = sf::Vector2f(static_cast<float>(vertex1.m_x), static_cast<float>(vertex1.m_y));
    triangle[0].color = m_color;
    triangle[1].position = sf::Vector2f(static_cast<float>(vertex2.m_x), static_cast<float>(vertex2.m_y));
    triangle[1].color = m_color;
    triangle[2].position = sf::Vector2f(static_cast<float>(vertex3.m_x), static_cast<float>(vertex3.m_y));
    triangle[2].color = m_color;

    m_window.draw(triangle);
  }

private:
  sf::RenderWindow& m_window;
  sf::Color m_color = sf::Color::Black;
  shapes::Point m_currentPoint = { 0, 0 };

  sf::Color uint32ToSFMLColor(Color color)
  {
    return {
      static_cast<sf::Uint8>((color >> 16) & 0xFF),  // R
      static_cast<sf::Uint8>((color >> 16) & 0xFF),  // G
      static_cast<sf::Uint8>((color >> 8) & 0xFF),   // B
      static_cast<sf::Uint8>(color & 0xFF),          // A
    };
  }

  void drawLine(shapes::Point from, shapes::Point to)
  {
    sf::Vertex line[] = {
      {sf::Vector2f(static_cast<float>(from.m_x), static_cast<float>(from.m_y)), m_color},
      {sf::Vector2f(static_cast<float>(to.m_x), static_cast<float>(to.m_y)), m_color}
    };
    m_window.draw(line, 2, sf::Lines);
  }
};
}

#endif //SFMLCANVAS_H