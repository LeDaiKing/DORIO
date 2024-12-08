#include "JumpyBlock.hpp"
#include "Utility.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <iostream>
#include "Dough.hpp"
#include "Entity.hpp"
// #include <iostream>


JumpyBlock::JumpyBlock(Type type, sf::Vector2f position)
: StaticBlock(type, position)
, nJumpForce(200)
, nIsPressed(false)
, nTimePressed(sf::Time::Zero)
{
}


void JumpyBlock::handleTopCollision(Entity& entity)
{
    sf::FloatRect bound = getBoundingRect();
    entity.setPosition(entity.getPosition().x, bound.top - entity.getBoundingRect().height / 2);
    if (entity.getVelocity().y > 0)
    {
        entity.setVelocity(entity.getVelocity().x, - entity.getVelocity().y);
    } else
    {
        entity.setVelocity(entity.getVelocity().x, 0.f);
    }
    entity.accelerate(0.f, -World::getGravity());
    entity.addVelocity(0.f, -nJumpForce);
    entity.setOnGround(true);
    if (!nIsPressed)
    {
        nSprite.setOrigin(nSprite.getOrigin().x, nSprite.getOrigin().y - 16.f);
        nSprite.scale(1.f, 0.5f);
        // setOrigin(getOrigin().x, getOrigin().y - 16.f);
        nTimePressed = sf::seconds(0.15f);
        nIsPressed = true;
    }
}

void JumpyBlock::setJumpForce(int force)
{
    nJumpForce = force;
}


void JumpyBlock::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // if (nIsPressed)
    // {
    //     nSprite.scale(1.f, 2.f);
    //     nIsPressed = false;
    // }
    if (nTimePressed > sf::Time::Zero)
    {
        nTimePressed -= dt;
        if (nTimePressed <= sf::Time::Zero)
        {
            // setOrigin(getOrigin().x, getOrigin().y + 16.f);
            nSprite.scale(1.f, 1/0.5f);
            nSprite.setOrigin(nSprite.getOrigin().x, nSprite.getOrigin().y + 16.f);
            nTimePressed = sf::Time::Zero;
            nIsPressed = false;
        }
    }

    StaticBlock::updateCurrent(dt, commands);
}