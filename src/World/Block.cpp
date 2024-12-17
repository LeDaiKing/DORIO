#include "Block.hpp"
#include "Entity.hpp"
#include "../Holder/ResourceHolder.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include "World.hpp"
#include "SnailShell.hpp"
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Textures::ID toTextureID(Block::Type type)
{
    switch (type)
    {
        case Block::Dirt:
            return Textures::Dirt;
        case Block::Breakable:
            return Textures::Breakable;
        case Block::LuckyBlock:
            return Textures::LuckyBlock;
        case Block::JumpyBlock:
            return Textures::JumpyBlock;
        case Block::SlideBlock:
            return Textures::Dirt;
        default:
            return Textures::Dirt;
    }
    return Textures::Dirt;
}


Block::Block(Type type, sf::Vector2f position)
: SpriteNode(TextureHolder::getInstance().get(toTextureID(type)))
,nType(type)
{
    centerOrigin(nSprite);
    setPosition(position);
}

unsigned int Block::getCategory() const
{
    return Category::Block;
}


sf::FloatRect Block::getBoundingRect() const
{
	return getWorldTransform().transformRect(nSprite.getGlobalBounds());
}

void Block::applyNormal(SceneNode& graph)
{
    if (graph.getCategory() & Category::Entity && !graph.isMarkedForRemoval())
    {
        assert(dynamic_cast<Entity*>(&graph) != nullptr);
        Entity& entity = static_cast<Entity&>(graph);

        sf::FloatRect entityHitBox = entity.getBoundingRect();
        sf::FloatRect bound = getBoundingRect();
        // entityHitBox.height += 3.f;

        // if (checkCollisionSide(entityHitBox, bound) == collision::Top && entity.getVelocity().y > 0)
        // {
        //     entity.accelerate(0.f, -World::getGravity());
        //     entity.setOnGround(true);
        //     // entity.updateClosestTopBlock(this);
        // }

        // bound.top += 5.f;


        collision::Side side = checkCollisionSide(entityHitBox, bound);
        int gravity = World::getGravity();

        if (entity.getCategory() & Category::PlayerDough)
        {
            if (side != collision::None) std:: cout << "Side: " << side << std::endl;
        }

        if (side == collision::Top && entity.getVelocity().y > 0)
        {
            // entity.move(0.f, bound.top - entityHitBox.top - entityHitBox.height);
            // entity.accelerate(0.f, -World::getGravity());
            entity.setOnGround(true);
            entity.updateClosestTopBlock(this);
            // handleTopCollision(entity);
        }
        else if (side == collision::Bottom && entity.getVelocity().y < 0)
        {
            // breakBlock(entity);
            entity.updateClosestBottomBlock(this);  
            
        }
        
        entityHitBox.height -= 3.f;
        side = checkCollisionSide(entityHitBox, bound);
        
        if (side == collision::Left && entity.getVelocity().x > 0)
        {
            entity.move(bound.left - entityHitBox.left - entityHitBox.width, 0.f);

            if ((entity.getCategory() & Category::Enemy))
            {
                if (dynamic_cast<SnailShell*>(&entity) != nullptr)
                {
                    SnailShell& enemy = static_cast<SnailShell&>(entity);
                    enemy.hitBlock();
                }
                else
                {
                    entity.setDirection(true);
                }
            }
            else
                entity.setVelocity(0.f, entity.getVelocity().y);
        }
        else if (side == collision::Right && entity.getVelocity().x < 0)
        {
            entity.move(bound.left + bound.width - entityHitBox.left, 0.f);

            if ((entity.getCategory() & Category::Enemy))
            {
                if (dynamic_cast<SnailShell*>(&entity) != nullptr)
                {
                    SnailShell& enemy = static_cast<SnailShell&>(entity);
                    enemy.hitBlock();
                }
                else
                {
                    entity.setDirection(false);
                }
            }
            else
                entity.setVelocity(0.f, entity.getVelocity().y);
        }

        

    }
    for (Ptr& child : graph.getChildren())
    {
        applyNormal(*child);
    }
}

// void Block::handleTopCollision(Entity& entity)
// {
//     sf::FloatRect bound = getBoundingRect();
//     entity.setPosition(entity.getPosition().x, bound.top - entity.getBoundingRect().height / 2);
//     entity.setVelocity(entity.getVelocity().x, 0.f);
//     entity.accelerate(0.f, -World::getGravity());
//     entity.setOnGround(true);
// }
