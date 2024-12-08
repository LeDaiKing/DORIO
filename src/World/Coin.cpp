#include "Coin.hpp"
#include "Dough.hpp"
#include "Animation.hpp"

Coin::Coin(Type type, sf::Vector2f position)
: Item(type, position)
{
    nAnimation.addAnimationState(0, 0, 8, sf::seconds(0.5f), sf::Vector2i(20, 20), true);
    nAnimation.setAnimationState(0);
}

void Coin::activate(Entity& player)
{
    nIsCollected = true;
}