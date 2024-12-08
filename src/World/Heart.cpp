#include "Heart.hpp"
#include "Dough.hpp"
#include "Animation.hpp"

Heart::Heart(Type type, sf::Vector2f position)
: Item(type, position)
{
    nAnimation.addAnimationState(0, 0, 1, sf::seconds(0.5f), sf::Vector2i(25, 25), true);
    nAnimation.setAnimationState(0);
}

void Heart::activate(Entity& player)
{
    nIsCollected = true;
}