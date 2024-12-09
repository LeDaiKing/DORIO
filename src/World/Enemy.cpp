#include "Enemy.hpp"
#include "Dough.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Category.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <iostream>
Textures::ID toTextureID(Enemy::Type type)
{
    switch (type)
    {
        case Enemy::CockRoach:
            return Textures::Enemy;
        case Enemy::Ghost:
            return Textures::Ghost;
    }
    return Textures::Enemy;
}

Enemy::Enemy(Type type, sf::Vector2f position)
: Entity(TextureHolder::getInstance().get(toTextureID(type)))
, nType(type)
, nAIState(None)
, nCurBehavior(0)
, nDestination(position)
, nDirLoop(1)
{
    setPosition(position);
}


unsigned int Enemy::getCategory() const
{
    return Category::Enemy;
}


Enemy::Type Enemy::getType()
{
    return nType;
}

void Enemy::setAIState(AIState state)
{
    nAIState = state;
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    Entity::updateCurrent(dt, commands);

    // if (nCurrentState == State::Dead)
    // {
    //     return;
    // }

    switch (nAIState)
    {
    case Patrolling:
        moveToPosition(dt);
        break;
    case Turning:
        turn();
        break;
    case Waiting:
        wait(dt);
        break;
    
    default:
        if (nBehaviors.size() == 0) break;

        nBehaviors[nCurBehavior]();
        nCurBehavior += nDirLoop;
        if (nCurBehavior == nBehaviors.size())
        {
            nCurBehavior = nBehaviors.size() - 1;
            nDirLoop = -1;
        }
        else if (nCurBehavior == -1)
        {
            nCurBehavior = 0;
            nDirLoop = 1;
        }
    }
}

void Enemy::addMoveBehavior(sf::Vector2f offset)
{
    // sf::Vector2f temp = nDestination + offset;
    nBehaviors.push_back([this, offset] ()
    {
        sf::Vector2f noffset = offset;
        noffset.x *= nDirLoop;
        noffset.y *= nDirLoop;
        nDestination = getPosition() + noffset;
        setAIState(Patrolling);
    });
    // nDestination = temp;
}

void Enemy::addWaitBehavior(sf::Time time)
{
    nBehaviors.push_back([this, time] ()
    {
        nWaitingTime = time;
        setAIState(Waiting);
    });
}

void Enemy::addTurnBehavior()
{
    nBehaviors.push_back([this] ()
    {
        setAIState(Turning);
    });
}

void Enemy::moveToPosition(sf::Time dt)
{   
    // std::cout << length(nDestination - getPosition()) << std::endl;
    // int sign = nDestination.x < getPosition().x ? -1 : 1;

    if (length(nDestination - getPosition()) < 3.f)
    {
        setVelocity(0.f, 0.f);
        setPosition(nDestination);
        setAIState(None);
        return;
    }
    
    float velocity = length(getVelocity());
    float firtion = length(0.8f * nSpeed);
    
    if (length(nDestination - getPosition()) > 0.5f * velocity * velocity / firtion)
    {
         if (nDestination.x < getPosition().x)  walk(true);
         else walk(false);
    }
}

void Enemy::wait(sf::Time time)
{
    nWaitingTime -= time;
    if (nWaitingTime <= sf::Time::Zero)
    {
        nWaitingTime = sf::Time::Zero;
        setAIState(None);
    }
}

void Enemy::turn()
{
    nDirection = !nDirection;
    nSprite.setFlipped(nDirection);
    setAIState(None);
}

void Enemy::isTargetInRange(sf::Vector2f target)
{
    //
}