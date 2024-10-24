#include "ldk.hpp"
double count = 0;
void RectangleShape::display(sf::RenderWindow& window) {
    
    count += 0.4;
    if (count > 360) {
        count = 0;
    }
    rotate(count);
    window.draw(*this);
}