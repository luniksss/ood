#ifndef SFMLCANVAS_H
#define SFMLCANVAS_H
#include <SFML/Graphics.hpp>
#include "ICanvas.h"

class SFMLCanvas: public ICanvas
{
public:
    explicit SFMLCanvas(sf::RenderWindow& window) : m_window(window)
    {
        m_texture.create(window.getSize().x, window.getSize().y);
        m_texture.clear(sf::Color::Transparent);
        m_texture.display();
    }

    void SetColor(uint32_t color) override
    {
        const sf::Color result = uint32ToSFMLColor(color);
        m_color = result;
    }

    void MoveTo(const double x, const double y) override
    {
        const shapes::Point newPoint(x, y);
        m_currentPoint = newPoint;
    }

    void LineTo(const double x, const double y) override
    {
        const shapes::Point destinationPoint(x, y);
        DrawLine(m_currentPoint, destinationPoint);
        m_currentPoint = destinationPoint;
    }

    void DrawLine(const shapes::Point from, const shapes::Point to) override
    {
        const sf::Vertex line[] =
        {
            {sf::Vector2f(static_cast<float>(from.m_x), static_cast<float>(from.m_y)), m_color},
            {sf::Vector2f(static_cast<float>(to.m_x), static_cast<float>(to.m_y)), m_color}
        };
        m_texture.draw(line, 2, sf::Lines);
    }

    void DrawEllipse(const double cx, const double cy, const double rx, const double ry) override
    {
        sf::CircleShape ellipse(1.f);
        ellipse.setPosition(cx - rx, cy - ry);
        ellipse.setScale(rx, ry);
        ellipse.setFillColor(m_color);
        m_texture.draw(ellipse);
    }

    void DrawRectangle(const shapes::Point topLeft, const double width, const double height) override
    {
        sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        rectangle.setPosition(static_cast<float>(topLeft.m_x), static_cast<float>(topLeft.m_y));
        rectangle.setFillColor(m_color);

        m_texture.draw(rectangle);
    }

    void DrawTriangle(const shapes::Point vertex1, const shapes::Point vertex2, const shapes::Point vertex3) override
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

//    void Display() override
//    {
//        sf::Event event{};
//        while (m_window.pollEvent(event))
//        {
//        if (event.type == sf::Event::Closed)
//          m_window.close();
//        }
//        m_texture.display();
//        const sf::Sprite sprite(m_texture.getTexture());
//        m_window.draw(sprite);
//        m_window.display();
//    }

private:
    sf::RenderWindow& m_window;
    sf::RenderTexture m_texture;
    sf::Color m_color = sf::Color::White;
    shapes::Point m_currentPoint;

    static sf::Color uint32ToSFMLColor(uint32_t color)
    {
        return {
          static_cast<sf::Uint8>((color >> 16) & 0xFF),  // R
          static_cast<sf::Uint8>((color >> 8) & 0xFF),   // G
          static_cast<sf::Uint8>(color & 0xFF),          // B
        };
    }
};

#endif //SFMLCANVAS_H