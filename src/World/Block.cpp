#include "Block.hpp"
#include "Entity.hpp"
#include "../Holder/ResourceHolder.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


Block::Block(sf::Vector2f position)
: SpriteNode(TextureHolder::getInstance().get(Textures::Dirt))
{
    centerOrigin(nSprite);
    nSprite.setPosition(position);
}

unsigned int Block::getCategory() const
{
    return Category::Block;
}

void Block::applyNormal(SceneNode& graph)
{
    if (graph.getCategory() & Category::Entity)
    {
        assert(dynamic_cast<Entity*>(&graph) != nullptr);
        Entity& entity = static_cast<Entity&>(graph);

        sf::FloatRect entityHitBox = entity.getHitBox();
        sf::FloatRect bound = getBoundingRect();

        collision::Side side = checkCollisionSide(entityHitBox, bound);


        if (side == collision::Top && entity.getVelocity().y > 0)
        {
            entity.setPosition(entity.getPosition().x, bound.top - entityHitBox.height / 2);
            entity.setVelocity(entity.getVelocity().x, 0.f);
            entity.accelerate(0.f, -512);
            entity.setOnGround(true);
        }
        else if (side == collision::Bottom && entity.getVelocity().y < 0)
        {
            entity.setPosition(entity.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
            entity.setVelocity(entity.getVelocity().x, 0.f);
            entity.accelerate(0.f, -512);

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

sf::FloatRect Block::getBoundingRect() const
{
	return getWorldTransform().transformRect(nSprite.getGlobalBounds());
}