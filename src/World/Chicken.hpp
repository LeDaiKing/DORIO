#pragma once
#include "Enemy.hpp"

class Dough;
class Chicken : public Enemy
{
    public:
        Chicken(Type type, sf::Vector2f position);
        virtual void attackPlayer(Dough& player);
        virtual void isTargetInRange(const sf::Vector2f& target);
    protected:
        virtual void setUpEntity() ;
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

    private:
        const sf::Vector2f* nTarget;
        float nRange;
};