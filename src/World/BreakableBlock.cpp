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

Textures::ID toTextureIDAni(BreakableBlock::Type type)
{
    switch (type)
    {
        case BreakableBlock::Type::Breakable1:
            return Textures::BreakAnimation1;
        case BreakableBlock::Type::Breakable2:
            return Textures::BreakAnimation2;
        default:
            break;
    }
    return Textures::Breakable1;
}

BreakableBlock::BreakableBlock(Type type, sf::Vector2f position)
: BouncingBlock(type, position)
, nIsBroken(false)
, nBreakAnimation(TextureHolder::getInstance().get(toTextureIDAni(type)))
{
    centerOrigin(nBreakAnimation);
    if (type == Type::Breakable1)
    {
        nBreakAnimation.addAnimationState(0, 0, 4, sf::seconds(0.3), sf::Vector2i(96, 32), false);
        nBreakAnimation.setAnimationState(0);
    }
    else
    {
        nBreakAnimation.addAnimationState(0, 0, 3, sf::seconds(0.3), sf::Vector2i(72, 63), false);
        nBreakAnimation.setAnimationState(0);
    }
}

void BreakableBlock::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nIsBroken)
    {
        nBreakAnimation.update(dt);
    }
    else
    {
        BouncingBlock::updateCurrent(dt, commands);
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
        BouncingBlock::drawCurrent(target, states);
    }
}

// void BreakableBlock::applyNormal(SceneNode& graph)
// {
//     if (nIsBroken)
//         return;

//     if (graph.getCategory() != Category::PlayerDough)
//     {
//         BouncingBlock::applyNormal(graph);
//         return;
//     }

//     assert(dynamic_cast<Dough*>(&graph) != nullptr);
//     Dough& player = static_cast<Dough&>(graph);

//     sf::FloatRect entityHitBox = player.getBoundingRect();
//     sf::FloatRect bound = getBoundingRect();

//     collision::Side side = checkCollisionSide(entityHitBox, bound);
//     if (side != collision::Bottom)
//     {
//         BouncingBlock::applyNormal(graph);
//         return;
//     }

//     player.updateCloestBlock(this);
//     // player.setPosition(player.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
//     // player.setVelocity(player.getVelocity().x, 30.f);
//     // nStateBounce = 1;

//     for(Ptr& child : getChildren())
//     {
//         applyNormal(*child);
//     }

// }

// void BreakableBlock::handleBottomCollision(Dough& player)
// {
//     if (nIsBroken)
//         return;

//     player.updateCloestBlock(this);
// }


void BreakableBlock::handleBottomCollision(Entity& player)
{
    if (nIsBroken)
        return;
    if (player.getCategory() & Category::PlayerDough)
    {
        Dough& player1 = static_cast<Dough&>(player);
        if (player1.getVelocity().y < -220.f && player1.getStateJump() == 1)
        {
            // nIsBroken = true;
            // nBreakAnimation.setAnimationState(0);
            breakBlock();
        }
    }
    BouncingBlock::handleBottomCollision(player);

    // else nStateBounce = 1;

    // sf::FloatRect bound = getBoundingRect();
    // player.setPosition(player.getPosition().x, bound.top + bound.height + player.getBoundingRect().height / 2);
    // player.setVelocity(player.getVelocity().x, 30.f);
}

bool BreakableBlock::isMarkedForRemoval() const
{
    return nIsBroken && nBreakAnimation.isFinished();
}

bool BreakableBlock::isBroken() const {
    return nIsBroken;
}

void BreakableBlock::breakBlock()
{
    nIsBroken = true;
    nBreakAnimation.setAnimationState(0);
}