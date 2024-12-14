#pragma once
#include "Enemy.hpp"

class CockRoach : public Enemy
{
    public:
        CockRoach(Type type, sf::Vector2f position);
    protected:
        virtual void setUpEntity() ;

};