#include "CockRoach.hpp"
#include "Dough.hpp"
#include "../ConfigLoader.hpp"

CockRoach::CockRoach(Type type, sf::Vector2f position)
: Enemy(type, position)
{
    setUpEntity();
    setAnimationState(State::Idle);
}


void CockRoach::setUpEntity()
{
    addAnimationState(State::Idle, 0, 4, sf::seconds(0.5), sf::Vector2i(32, 32), true);
    addAnimationState(State::Dead, 32, 5, sf::seconds(0.8), sf::Vector2i(32, 32), false);
    // addAnimationState(State::Walk, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    // addAnimationState(State::Hit, 0, 5, sf::seconds(0.5), sf::Vector2i(42, 28), false);
    // addAnimationState(State::Dead, 0, 5, sf::seconds(0.5), sf::Vector2i(44, 28), false);
   
    const nlohmann::json& config = ConfigLoader::getInstance().getConfig("Enemy/CockRoach");
   
   nSprite.turnInverse();
    nHitBox = toVector2<float>(config["HitBox"]);
    nSpeed = toVector2<float>(config["Speed"]);
    nMaxVelocity = toVector2<float>(config["MaxVelocity"]);
    nJumpVelocity = config["JumpVelocity"];
}

