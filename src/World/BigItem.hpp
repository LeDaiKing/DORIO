#pragma once
#include "Item.hpp"
class Dough;

class BigItem : public Item
{
    public:
        BigItem(Type type, sf::Vector2f position);
        virtual void activate(Entity& player) ;
};