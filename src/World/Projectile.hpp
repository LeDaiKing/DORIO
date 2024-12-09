#pragma once
#include "Item.hpp"
class Entity;

class Projectile : public Item
{
    public:
        enum AniState
        {
            Fire,
            Explode,
        };
    public:
        Projectile(Type type, sf::Vector2f position);
        virtual void activate(Entity& player);
        void setSpeed(float speed);
        void setDamage(unsigned int damage);
        void setTargetCategory(unsigned int category);
        void setTargetDirection(sf::Vector2f direction);
        void setTargetPosition(sf::Vector2f position);
        virtual bool isMarkedForRemoval() const;
        void destroy();
        virtual sf::FloatRect getBoundingRect() const;
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void setAnimationState(AniState state);
    private:
        sf::Vector2f nVelocity;
        float nSpeed;
        unsigned int nDamage;
        unsigned int nTargetCategory;
        sf::Vector2f nHitBox;
};