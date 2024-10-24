#include <SFML/Graphics.hpp>
#include "ldk.hpp" 

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //draw a circle
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);
        RectangleShape rectangle(sf::Vector2f(100, 50), sf::Color::Red);
        rectangle.setPosition(400, 300);
        rectangle.display(window);

        // window.draw(shape);
        // window.display();

        window.display();
        window.clear();
    }
    return 0;
}
