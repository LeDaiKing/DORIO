#include "Item.hpp"
#include "../Holder/ResourceHolder.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Textures::ID toTextureID(Item::Type type)
{
    switch (type)
    {
        case Item::Coin:
            return Textures::Coin;
        case Item::Heart:
            return Textures::Heart;
        case Item::Big:
            return Textures::BigItem;
        case Item::FireBig:
            return Textures::Heart;
        case Item::FireBall:
            return Textures::FireBall;
        default:
            break;
    }
    return Textures::Coin;
}

Item::Item(Type type, sf::Vector2f position)
: nAnimation(TextureHolder::getInstance().get(toTextureID(type)))
, nType(type)
, nIsCollected(false)
{
    centerOrigin(nAnimation);
    setPosition(position);
}

unsigned int Item::getCategory() const
{
    return Category::Item;
}

sf::FloatRect Item::getBoundingRect() const
{
    return getWorldTransform().transformRect(nAnimation.getGlobalBounds());
}

bool Item::isMarkedForRemoval() const
{
    return nIsCollected;
}

void Item::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (!isMarkedForRemoval())
    {
        nAnimation.update(dt);
    }
}

void Item::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!isMarkedForRemoval())
    {
        target.draw(nAnimation, states);
    }
}