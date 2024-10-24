// class rectangleshpae can rotate

#include <iostream>
#include <SFML/Graphics.hpp>

class RectangleShape : public sf::RectangleShape {
public:
    RectangleShape() : sf::RectangleShape() {}
    RectangleShape(const sf::Vector2f& size) : sf::RectangleShape(size) {}
    RectangleShape(const sf::Vector2f& size, const sf::Color& color) : sf::RectangleShape(size) {
        setFillColor(color);
    }
    //display rectangle rotate around the center
    void display(sf::RenderWindow& window);
};