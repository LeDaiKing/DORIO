#include "Block.hpp"
#include "Entity.hpp"
#include "../Holder/ResourceHolder.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include "World.hpp"
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
    if (graph.getCategory() & Category::Entity)
    {
        assert(dynamic_cast<Entity*>(&graph) != nullptr);
        Entity& entity = static_cast<Entity&>(graph);

        sf::FloatRect entityHitBox = entity.getBoundingRect();
        sf::FloatRect bound = getBoundingRect();
        // bound.top -= 1.f;

        collision::Side side = checkCollisionSide(entityHitBox, bound);
        int gravity = World::getGravity();

        if (side == collision::Top && entity.getVelocity().y > 0)
        {
            handleTopCollision(entity);
        }
        else if (side == collision::Bottom && entity.getVelocity().y < 0)
        {
            // breakBlock(entity);
            if (entity.getCategory() == Category::PlayerDough)
            {
                Dough& player = static_cast<Dough&>(entity);
                player.updateCloestBlock(this);
                // handleBottomCollision(player);
            }
            else
            {
                entity.setPosition(entity.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
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
