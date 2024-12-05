#include "Coin.hpp"
#include "Dough.hpp"

Coin::Coin(sf::Vector2f position)
: Item(Type::Coin, sf::Vector2i(20, 20), 6, sf::seconds(0.5))
{
    setPosition(position);
}

void Coin::collectedBy(Dough& player)
{
    nIsCollected = true;
}