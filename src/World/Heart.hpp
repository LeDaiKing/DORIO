#pragma once
#include "Item.hpp"
class Dough;

class Heart : public Item
{
    public:
        Heart(Type type, sf::Vector2f position);
        virtual void activate(Entity& player) ;
};