#pragma once
#include "Item.hpp"
class Dough;

class Coin : public Item
{
    public:
        Coin(Type type, sf::Vector2f position);
        virtual void activate(Entity& player) ;
};