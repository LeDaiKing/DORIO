#pragma once
#include "Enemy.hpp"

class Dough;
class SnailShell : public Enemy
{
    public:
        SnailShell(Type type, sf::Vector2f position);
        // virtual void getDamage(int damage);
        virtual void attackPlayer(Dough& player);
        void attackEnemy(Enemy& enemy);
        void hitBlock();
        virtual bool isAutoAI();
        // virtual void isTargetInRange(const sf::Vector2f& target);
        bool isPushed();
    protected:
        virtual void setUpEntity() ;
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

    private:
        bool nPush;
};