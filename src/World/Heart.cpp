#include "Heart.hpp"
#include "Dough.hpp"
#include "Animation.hpp"

Heart::Heart(Type type, sf::Vector2f position)
: Item(type, position)
{
    nAnimation.addAnimationState(0, 0, 7, sf::seconds(0.7f), sf::Vector2i(15, 30), true);
    nAnimation.setAnimationState(0);
}

void Heart::activate(Entity& player)
{
    nIsCollected = true;
    Dough& dough = static_cast<Dough&>(player);
    dough.addHitPoints(1);
    // // dough.growUPBig();
    // dough.growUPFireBig();
}