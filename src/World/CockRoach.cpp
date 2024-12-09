#include "CockRoach.hpp"
#include "Dough.hpp"

CockRoach::CockRoach(Type type, sf::Vector2f position)
: Enemy(type, position)
{
    setUpEntity();
    setAnimationState(State::Idle);
}


void CockRoach::setUpEntity()
{
    addAnimationState(State::Idle, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    // addAnimationState(State::Walk, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    addAnimationState(State::Hit, 0, 5, sf::seconds(0.8), sf::Vector2i(44, 30), false);
    addAnimationState(State::Dead, 0, 5, sf::seconds(0.6), sf::Vector2i(44, 30), false);
    nHitBox = sf::Vector2f(35.f, 25.f);
    nSpeed = sf::Vector2f(576.f, 0.f);
    nMaxVelocity = sf::Vector2f(64.f, 64.f);
    nJumpVelocity = 0;
}

void CockRoach::attackPlayer(Dough& player)
{
    if (nCurrentState == State::Dead)
        return;
    player.getDamage(1);
    if (player.getPosition().x < getPosition().x)
    {
        player.setVelocity(-512.f, 0);
    }
    else
    {
        player.setVelocity(512.f, 0);
    }
}
