#include "BigItem.hpp"
#include "Dough.hpp"
#include "Animation.hpp"

BigItem::BigItem(Type type, sf::Vector2f position)
: Item(type, position)
{
    nAnimation.addAnimationState(0, 0, 12, sf::seconds(0.7f), sf::Vector2i(24, 39), true);
    nAnimation.setAnimationState(0);
}

void BigItem::activate(Entity& player)
{
    nIsCollected = true;
    Dough& dough = static_cast<Dough&>(player);
    // dough.growUPBig();
    dough.growUPBig();
}