#include "Chicken.hpp"
#include "Utility.hpp"
#include "Dough.hpp"
#include "../ConfigLoader.hpp"
// #include <iostream>

Chicken::Chicken(Type type, sf::Vector2f position)
: Enemy(type, position)
// , nRange(100.f)
{
    setUpEntity();
    setAnimationState(State::Idle);
}


void Chicken::setUpEntity()
{
    addAnimationState(State::Idle, 34, 13, sf::seconds(0.8), sf::Vector2i(32, 34), true);
    addAnimationState(State::Walk, 68, 14, sf::seconds(0.65), sf::Vector2i(32, 34), true);
    addAnimationState(State::Hit, 0, 5, sf::seconds(0.5), sf::Vector2i(32, 34), false);
    addAnimationState(State::Dead, 0, 5, sf::seconds(0.5), sf::Vector2i(32, 34), false);
    nSprite.turnInverse();
    
    const nlohmann::json& config = ConfigLoader::getInstance().getConfig("Enemy/Chicken");
    
    nHitBox = toVector2<float>(config["HitBox"]);
    nSpeed = toVector2<float>(config["Speed"]);
    nMaxVelocity = toVector2<float>(config["MaxVelocity"]);
    nJumpVelocity = config["JumpVelocity"];
    nRange = config["Range"];
}

void Chicken::isTargetInRange(const sf::Vector2f& target)
{
    if (nTarget != nullptr) return;
    if (length(target - getPosition()) < nRange)
    {
        nTarget = &target;
        setAIState(AIState::Chasing);
    }
}

void Chicken::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nCurrentState != State::Dead)
    {
        if (nAIState == AIState::Chasing)
        {
            nDestination = *nTarget;
            if (abs(nDestination.x - getPosition().x) > 20.f)
            moveToPosition(dt);
        }
    }
    Enemy::updateCurrent(dt, commands);

    // std::cout << nSprite.getCurrentAnimationID() << std::endl;  
}

void Chicken::attackPlayer(Dough& player)
{
    if (nCurrentState == State::Dead)
        return;
    player.getDamage(1);
    if (player.getPosition().x < getPosition().x)
    {
        player.setVelocity(-512.f, 0);
        setVelocity(300.f, 0);
    }
    else
    {
        player.setVelocity(512.f, 0);
        setVelocity(-300.f, 0);
    }
}