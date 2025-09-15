#include <SFML/Graphics.hpp>
#include "service/DrawingController.h"
#include "picture/Picture.h"
#include "gfx/SFMLCanvas.h"

#include <iostream>

int main()
{
    try
    {
        sf::RenderWindow window(sf::VideoMode(1200, 1000), "Shapes");
        window.setFramerateLimit(60);
        gfx::SFMLCanvas canvas(window);

        Picture picture;
        DrawingCommandHandler handler(std::cout, canvas, picture);
        DrawingController controller(std::cin, handler);

        controller.StartDrawing();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}