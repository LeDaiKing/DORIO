#pragma once
#include "Enemy.hpp"

class SnailShell;
class Snail : public Enemy
{
    public:
        Snail(Type type, sf::Vector2f position);
        // virtual void attackPlayer(Dough& player) ;
        // virtual void isTargetInRange(const sf::Vector2f& target);
    protected:
        virtual void setUpEntity() ;
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void setAnimationState(State type);

    private:
        // const sf::Vector2f* nTarget;
        // float nRange;
        bool nOutShell;
};