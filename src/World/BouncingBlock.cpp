#include "BouncingBlock.hpp"
#include "Utility.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <iostream>
#include "Dough.hpp"
#include "Category.hpp"


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
     entity.move(0.f, getBoundingRect().top - entity.getBoundingRect().top - entity.getBoundingRect().height);
    entity.setVelocity(entity.getVelocity().x, 0.f);
}

void BouncingBlock::handleBottomCollision(Entity& player)
{
    sf::FloatRect bound = getBoundingRect();
    player.move(0.f, bound.top + bound.height - player.getBoundingRect().top);
    player.setVelocity(player.getVelocity().x, 0.f);
    if (player.getCategory() & Category::PlayerDough)
    {
        nStateBounce = 1;
    }
}
