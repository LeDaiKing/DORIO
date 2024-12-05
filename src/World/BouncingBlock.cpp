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

void BouncingBlock::updateCurrent(sf::Time dt)
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

void BouncingBlock::applyNormal(SceneNode& graph)
{
    if (graph.getCategory() & Category::Entity)
    {
        assert(dynamic_cast<Entity*>(&graph) != nullptr);
        Entity& entity = static_cast<Entity&>(graph);

        sf::FloatRect entityHitBox = entity.getBoundingRect();
        sf::FloatRect bound = getBoundingRect();

        collision::Side side = checkCollisionSide(entityHitBox, bound);
        int gravity = World::getGravity();

        if (side == collision::Top && entity.getVelocity().y > 0)
        {
            entity.setPosition(entity.getPosition().x, bound.top - entityHitBox.height / 2);
            entity.setVelocity(entity.getVelocity().x, 0.f);
            entity.accelerate(0.f, -gravity);
            entity.setOnGround(true);
        }
        else if (side == collision::Bottom && entity.getVelocity().y < 0)
        {
            // breakBlock(entity);
            entity.setPosition(entity.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
            if (entity.getCategory() == Category::PlayerDough)
            {
                entity.setVelocity(entity.getVelocity().x, 30.f);
                nStateBounce = 1;
            }
            else
            {
                entity.setVelocity(entity.getVelocity().x, 0.f);
            }
            
        }
        else if (side == collision::Left && entity.getVelocity().x > 0)
        {
            entity.setPosition(bound.left - entityHitBox.width / 2, entity.getPosition().y);
            entity.setVelocity(0.f, entity.getVelocity().y);
        }
        else if (side == collision::Right && entity.getVelocity().x < 0)
        {
            entity.setPosition(bound.left + bound.width + entityHitBox.width / 2, entity.getPosition().y);
            entity.setVelocity(0.f, entity.getVelocity().y);
        }

        

    }
    for (Ptr& child : graph.getChildren())
    {
        applyNormal(*child);
    }
}

