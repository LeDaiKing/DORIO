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
        case Block::Floor1:
            return Textures::Floor1;
        case Block::UnderFloor1:
            return Textures::UnderFloor1;
        case Block::StaticBlock1:
            return Textures::StaticBlock1;
        case Block::Breakable1:
            return Textures::Breakable1;
        case Block::LuckyBlock1:
            return Textures::LuckyBlock1;
        case Block::Floor2:
            return Textures::Floor2;
        case Block::UnderFloor2:
            return Textures::UnderFloor2;
        case Block::StaticBlock2:
            return Textures::StaticBlock2;
        case Block::Breakable2:
            return Textures::Breakable2;
        case Block::LuckyBlock2:
            return Textures::LuckyBlock2;
        case Block::SlideBlock:
            return Textures::SlideBlock;
        case Block::JumpyBlock:
            return Textures::JumpyBlock;
    }
    return Textures::Floor1;
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

bool Block::isBroken() const { return false; }

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


void Block::save(std::ofstream& file)
{
    if (isBroken()) 
    {
        int type = -1;
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        return;
    }
    file.write(reinterpret_cast<const char*>(&nType), sizeof(nType));
    sf::Vector2f pos = getPosition();
    file.write(reinterpret_cast<char*>(&pos), sizeof(pos));
}
void Block::load(std::ifstream& file)
{
    // sf::Vector2f position;
    // file.read(reinterpret_cast<char*>(&position), sizeof(position));
    // setPosition(position);
}