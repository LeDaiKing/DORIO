#include "FireBigItem.hpp"
#include "Dough.hpp"
#include "Animation.hpp"

FireBigItem::FireBigItem(Type type, sf::Vector2f position)
: Item(type, position)
{
    nAnimation.addAnimationState(0, 0, 14, sf::seconds(0.7f), sf::Vector2i(16, 51), true);
    nAnimation.setAnimationState(0);
}

void FireBigItem::activate(Entity& player)
{
    nIsCollected = true;
    Dough& dough = static_cast<Dough&>(player);
    // dough.growUPBig();
    dough.growUPFireBig();
}