#include "BigItem.hpp"
#include "Dough.hpp"
#include "Animation.hpp"

BigItem::BigItem(Type type, sf::Vector2f position)
: Item(type, position)
{
    nAnimation.addAnimationState(0, 0, 1, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
    nAnimation.setAnimationState(0);
}

void BigItem::activate(Entity& player)
{
    nIsCollected = true;
    Dough& dough = static_cast<Dough&>(player);
    // dough.growUPBig();
    dough.growUPBig();
}