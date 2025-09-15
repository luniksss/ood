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
  explicit SFMLCanvas(sf::RenderWindow& window) : m_window(window)
  {
    m_texture.create(window.getSize().x, window.getSize().y);
    m_texture.clear(sf::Color::Transparent);
    m_texture.display();
  }

  void SetColor(const Color color) override
  {
    sf::Color result = uint32ToSFMLColor(color);
    m_color = result;
  }

  void MoveTo(double x, double y) override
  {
    const Point newPoint(x, y);
    m_currentPoint = newPoint;
  }

  void LineTo(double x, double y) override
  {
    const Point destinationPoint(x, y);
    drawLine(m_currentPoint, destinationPoint);
    m_currentPoint = destinationPoint;
  }

  void DrawEllipse(double cx, double cy, double rx, double ry) override
  {
    sf::CircleShape ellipse(1.f);
    ellipse.setPosition(cx - rx, cy - ry);
    ellipse.setScale(rx, ry);
    ellipse.setFillColor(m_color);
    m_texture.draw(ellipse);
  }

  void DrawText(double left, double top, double fontSize, const std::string& text) override
  {
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded)
    {
      if (!font.loadFromFile(FONT_FILE_PATH))
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
    m_texture.draw(sfText);
  }

  void DrawRectangle(Point topLeft, double width, double height) override
  {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    rectangle.setPosition(static_cast<float>(topLeft.m_x), static_cast<float>(topLeft.m_y));
    rectangle.setFillColor(m_color);

    m_texture.draw(rectangle);
  }

  void DrawTriangle(Point vertex1, Point vertex2, Point vertex3) override
  {
    sf::VertexArray triangle(sf::Triangles, 3);

    triangle[0].position = sf::Vector2f(static_cast<float>(vertex1.m_x), static_cast<float>(vertex1.m_y));
    triangle[0].color = m_color;
    triangle[1].position = sf::Vector2f(static_cast<float>(vertex2.m_x), static_cast<float>(vertex2.m_y));
    triangle[1].color = m_color;
    triangle[2].position = sf::Vector2f(static_cast<float>(vertex3.m_x), static_cast<float>(vertex3.m_y));
    triangle[2].color = m_color;

    m_texture.draw(triangle);
  }

  void Display() override
  {
    sf::Event event{};
    while (m_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        m_window.close();
    }
    m_texture.display();
    sf::Sprite sprite(m_texture.getTexture());
    m_window.draw(sprite);
    m_window.display();
  }

private:
  const char* FONT_FILE_PATH = R"(D:\Luniks\uni\ood\labs\lab1\task4\gfx\font\Merriweather-Regular.ttf)";
  sf::RenderWindow& m_window;
  sf::RenderTexture m_texture;
  sf::Color m_color = sf::Color::White;
  Point m_currentPoint;

  static sf::Color uint32ToSFMLColor(Color color)
  {
    return {
      static_cast<sf::Uint8>((color >> 16) & 0xFF),  // R
      static_cast<sf::Uint8>((color >> 8) & 0xFF),   // G
      static_cast<sf::Uint8>(color & 0xFF),          // B
  };
  }

  void drawLine(Point from, Point to)
  {
    const sf::Vertex line[] = {
      {sf::Vector2f(static_cast<float>(from.m_x), static_cast<float>(from.m_y)), m_color},
      {sf::Vector2f(static_cast<float>(to.m_x), static_cast<float>(to.m_y)), m_color}
    };
    m_texture.draw(line, 2, sf::Lines);
  }
};
}

#endif //SFMLCANVAS_H