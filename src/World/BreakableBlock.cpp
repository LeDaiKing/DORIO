#include "BreakableBlock.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Utility.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <iostream>

BreakableBlock::BreakableBlock(sf::Vector2f position)
: Block(Type::Breakable, position)
, nBreakAnimation(TextureHolder::getInstance().get(Textures::BreakAnimation))
, nIsBroken(false)
{
    centerOrigin(nBreakAnimation);
    nBreakAnimation.addAnimationState(0, 0, 3, sf::seconds(0.3), sf::Vector2i(72, 63), false);
    nBreakAnimation.setAnimationState(0);
}

void BreakableBlock::updateCurrent(sf::Time dt)
{
    if (nIsBroken)
    {
        nBreakAnimation.update(dt);
        if (!nBreakAnimation.isFinished())
        {
            // nIsBroken = false;
            // nBreakAnimation.setAnimationState(0);
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
            entity.setPosition(entity.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
            entity.setVelocity(entity.getVelocity().x, 30.f);
            nIsBroken = true;
            // entity.accelerate(0.f, -gravity);

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