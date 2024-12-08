#include "ItemFactory.hpp"
#include "Coin.hpp"
#include "Heart.hpp"
#include "../Holder/ResourceHolder.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"

std::unique_ptr<Item> ItemFactory::createItem(Item::Type type, sf::Vector2f position)
{
    switch (type)
    {
        case Item::Coin:
            return std::make_unique<Coin>(type, position);
        case Item::Heart:
            return std::make_unique<Heart>(type, position);
    }
    return nullptr;
}

std::unique_ptr<Animation> ItemFactory::createAppearAnimation(Item::Type type)
{
    std::unique_ptr<Animation> animation;
    switch (type)
    {
        case Item::Coin:
            animation = std::make_unique<Animation>(TextureHolder::getInstance().get(Textures::CoinAnimation));
            animation->addAnimationState(0, 0, 8, sf::seconds(0.7), sf::Vector2i(20, 160), false);
            animation->setAnimationState(0);
            return animation;
        case Item::Heart:
            animation = std::make_unique<Animation>(TextureHolder::getInstance().get(Textures::HeartAnimation));
            animation->addAnimationState(0, 0, 4, sf::seconds(0.6), sf::Vector2i(25, 64), false);
            animation->setAnimationState(0);
            return animation;
    }
    return nullptr;
}