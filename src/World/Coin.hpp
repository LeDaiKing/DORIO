#pragma once
#include "Item.hpp"
class Dough;

class Coin : public Item
{
    public:
        Coin(sf::Vector2f position);
        virtual void collectedBy(Dough& player) override;
};