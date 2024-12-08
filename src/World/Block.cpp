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
        case Block::Stone:
            return Textures::Stone;
        case Block::UnderDirt:
            return Textures::UnderDirt;
        case Block::Gold:
            return Textures::Gold;
        case Block::Chimney:
            return Textures::Chimney;
        case Block::Wonder:
            return Textures::Wonder;
        case Block::Move:
            return Textures::Move;
        case Block::Break:
            return Textures::Break;
        case Block::Loxo:
            return Textures::Loxo;

        
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