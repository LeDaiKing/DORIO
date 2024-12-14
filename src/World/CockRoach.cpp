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
    addAnimationState(State::Idle, 0, 4, sf::seconds(0.5), sf::Vector2i(42, 28), true);
    // addAnimationState(State::Walk, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    // addAnimationState(State::Hit, 0, 5, sf::seconds(0.5), sf::Vector2i(42, 28), false);
    // addAnimationState(State::Dead, 0, 5, sf::seconds(0.5), sf::Vector2i(44, 28), false);
    nSprite.turnInverse();
    nHitBox = sf::Vector2f(25.f, 20.f);
    nSpeed = sf::Vector2f(576.f, 0.f);
    nMaxVelocity = sf::Vector2f(64.f, 64.f);
    nJumpVelocity = 0;
}

