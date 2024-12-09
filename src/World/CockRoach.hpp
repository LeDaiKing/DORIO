#include "Enemy.hpp"

class CockRoach : public Enemy
{
    public:
        CockRoach(Type type, sf::Vector2f position);
        virtual void attackPlayer(Dough& player) ;
    protected:
        virtual void setUpEntity() ;

};