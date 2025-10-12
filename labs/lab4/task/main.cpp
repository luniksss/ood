#include <SFML/Graphics.hpp>
#include "canvas/SFMLCanvas.h"
#include "designer/Designer.h"
#include "painter/Painter.h"
#include "shapes/ShapeFactory.h"
#include "Client.h"
#include <iostream>

int main()
{
    try
    {
        sf::RenderWindow window(sf::VideoMode(1200, 1000), "Shapes");
        window.setFramerateLimit(60);
        SFMLCanvas canvas(window);

        ShapeFactory shapeFactory;
        Designer designer(shapeFactory);
        Painter painter;

        const Client client(designer);
        client.DescribeDraft(std::cin, canvas, painter);
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
