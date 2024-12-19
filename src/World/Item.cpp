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
            return Textures::Heart;
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


void Item::save(std::ofstream& file)
{
    int type = nType;
    file.write(reinterpret_cast<char*>(&type), sizeof(type));
    if (nType == FireBall) return;
    sf::Vector2f pos = getPosition();
    file.write(reinterpret_cast<char*>(&pos), sizeof(pos));
    file.write(reinterpret_cast<char*>(&nIsCollected), sizeof(nIsCollected));
}

void Item::load(std::ifstream& file)
{
    file.read(reinterpret_cast<char*>(&nIsCollected), sizeof(nIsCollected));
}