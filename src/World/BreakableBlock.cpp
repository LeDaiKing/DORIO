#include "BreakableBlock.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Utility.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <iostream>
#include "Dough.hpp"

BreakableBlock::BreakableBlock(sf::Vector2f position)
: Block(Type::Breakable, position)
, nBreakAnimation(TextureHolder::getInstance().get(Textures::BreakAnimation))
, nIsBroken(false)
{
    centerOrigin(nBreakAnimation);
    nBreakAnimation.addAnimationState(0, 0, 3, sf::seconds(0.3), sf::Vector2i(72, 63), false);
    nBreakAnimation.setAnimationState(0);
    nMoveSpeed = 9/0.185;
    nOriginPosition = position;
    nDestination = position; nDestination.y -= 9;
    nStateMove = 0;
}

void BreakableBlock::updateCurrent(sf::Time dt)
{
    if (nIsBroken)
    {
        nBreakAnimation.update(dt);
    }

    if (nStateMove == 1)
    {
        move(0, -nMoveSpeed * dt.asSeconds());
        if (getPosition().y <= nDestination.y)
        {
            setPosition(nDestination);
            nStateMove = 2;
        }
    }
    else if (nStateMove == 2)
    {
        move(0, nMoveSpeed * dt.asSeconds());
        if (getPosition().y >= nOriginPosition.y)
        {
            setPosition(nOriginPosition);
            nStateMove = 0;
        }
    }
   
}

void BreakableBlock::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (nIsBroken)
    {
        if (!nBreakAnimation.isFinished())
        target.draw(nBreakAnimation, states);
    }
    else
    {
        SpriteNode::drawCurrent(target, states);
    }
}

void BreakableBlock::applyNormal(SceneNode& graph)
{
    if (nIsBroken)
        return;

    if (graph.getCategory() != Category::PlayerDough)
    {
        Block::applyNormal(graph);
        return;
    }

    assert(dynamic_cast<Dough*>(&graph) != nullptr);
    Dough& entity = static_cast<Dough&>(graph);

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
        // entity.setPosition(entity.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
        // entity.setVelocity(entity.getVelocity().x, 30.f);

        // nIsBroken = true;
        // entity.accelerate(0.f, -gravity);
        entity.updateCloestBlock(this);
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

    

    for (Ptr& child : graph.getChildren())
    {
        applyNormal(*child);
    }
}


void BreakableBlock::breakBlock(Dough& player)
{
    if (player.getVelocity().y < -220.f)
    {
        nIsBroken = true;
        nBreakAnimation.setAnimationState(0);

    }
    else
    {
        nStateMove = 1;
    }

    sf::FloatRect entityHitBox = player.getBoundingRect();
    sf::FloatRect bound = getBoundingRect();

    player.setPosition(player.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
    player.setVelocity(player.getVelocity().x, 30.f);
}

bool BreakableBlock::isMarkedForRemoval() const
{
    return nIsBroken && nBreakAnimation.isFinished();
}