#pragma once
#include "Enemy.hpp"

class Ghost : public Enemy
{
    public:
        Ghost(Type type, sf::Vector2f position);
        virtual void attackPlayer(Dough& player) ;
        // virtual void addMoveBehavior(sf::Vector2f offset);
        virtual void isTargetInRange(const sf::Vector2f& target);
        void setFireCooldown(sf::Time time);
        void setFireRange(float range);
        void setFireSpeed(float speed);
        virtual void getDamage(int damage);
        // void
    protected:
        virtual void setUpEntity() ;
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void moveToPosition(sf::Time dt);
        void fire(CommandQueue& commands);
        virtual void setAnimationState(State type);

    private:
        sf::Vector2f direction;
        float nGSpeed;
        float nMaxGVelocity;
        float nFireRange;
        float nFireSpeed;
        sf::Time nFireCooldown;
        sf::Time nCurFireCooldown;
		sf::Vector2f nTarget;
        AIState nPreAIState;
        sf::Time nTimeDisappear;
};