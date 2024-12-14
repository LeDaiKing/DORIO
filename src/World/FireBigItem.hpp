#pragma once
#include "Item.hpp"
class Dough;

class FireBigItem : public Item
{
    public:
        FireBigItem(Type type, sf::Vector2f position);
        virtual void activate(Entity& player) ;
};