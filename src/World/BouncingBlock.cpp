#include "BouncingBlock.hpp"
#include "Utility.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <iostream>
#include "Dough.hpp"


BouncingBlock::BouncingBlock(Type type, sf::Vector2f position)
: Block(type, position)
{
    nOriginPosition = position;
    nDestination = position; nDestination.y -= 9;
    nBounceSpeed = 9/0.185f;
    nStateBounce = 0;
}

void BouncingBlock::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nStateBounce == 1)
    {
        move(0, -nBounceSpeed * dt.asSeconds());
        if (getPosition().y <= nDestination.y)
        {
            setPosition(nDestination);
            nStateBounce = 2;
        }
    }
    else if (nStateBounce == 2)
    {
        move(0, nBounceSpeed * dt.asSeconds());
        if (getPosition().y >= nOriginPosition.y)
        {
            setPosition(nOriginPosition);
            nStateBounce = 0;
        }
    }
   
}

void BouncingBlock::handleTopCollision(Entity& entity)
{
    sf::FloatRect bound = getBoundingRect();
    entity.setPosition(entity.getPosition().x, bound.top - entity.getBoundingRect().height / 2);
    entity.setVelocity(entity.getVelocity().x, 0.f);
    entity.accelerate(0.f, -World::getGravity());
    entity.setOnGround(true);
}

void BouncingBlock::handleBottomCollision(Dough& player)
{
    sf::FloatRect bound = getBoundingRect();
    player.setPosition(player.getPosition().x, bound.top + bound.height + player.getBoundingRect().height / 2);
    player.setVelocity(player.getVelocity().x, 0.f);
    nStateBounce = 1;
}
