#include "LuckyBlock.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Utility.hpp"
#include "Dough.hpp"
#include "World.hpp"
#include "ItemFactory.hpp"
// #include <iostream>

LuckyBlock::LuckyBlock(Type type, sf::Vector2f position)
: BouncingBlock(type, position)
, nIsEmpty(true)
, nIsDropping(false)
, nTexture(TextureHolder::getInstance().get(Textures::StaticLuckyBlock))
{
}


void LuckyBlock::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (!nIsEmpty)
    {
        BouncingBlock::updateCurrent(dt, commands);
    }
    if (nIsDropping)
    {
        // commands.push(nItems.top().first);
        // nQueueItems.push_back(std::move(nItems.top().second));
        nQueueItems.push_back(std::move(nItems.top()));
        nQueueItems.back().second->setPosition(0, -16);
        nItems.pop();
        nIsDropping = false;
    }
    if (nItems.empty() && nStateBounce == 0)
    {
        nIsEmpty = true;
        nSprite.setTexture(nTexture);
    }

    for (auto it = nQueueItems.begin(); it != nQueueItems.end();)
    {
        if ((*it).second->isFinished())
        {
            commands.push((*it).first);
            it = nQueueItems.erase(it);
        }
        else
        {
            (*it).second->update(dt);
            ++it;
        }
    }
}

void LuckyBlock::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    BouncingBlock::drawCurrent(target, states);
    for (const auto& ani : nQueueItems)
    {
        target.draw(*ani.second, states);
    }
}

void LuckyBlock::addItem(Item::Type type)
{
    nIsEmpty = false;
    Command command;
    command.category = Category::ItemScene;
    command.action = [this, type](SceneNode& graph, sf::Time)
    {
        if (type == Item::Coin)
        {
            return;
        }
        std::unique_ptr<Item> item = ItemFactory::createItem(type, getWorldPosition() + sf::Vector2f(0.f, -32.f));
        graph.attachChild(std::move(item));
    };
    ItemPair itemPair;
    itemPair.first = command;
    itemPair.second = ItemFactory::createAppearAnimation(type);

    nItems.push(std::move(itemPair));
}

void LuckyBlock::handleBottomCollision(Dough& player)
{
    dropItem();
    if (nIsEmpty)
    {
        sf::FloatRect bound = getBoundingRect();
        player.setPosition(player.getPosition().x, bound.top + bound.height + player.getBoundingRect().height / 2);
        player.setVelocity(player.getVelocity().x, 0);
        return;
    }
    BouncingBlock::handleBottomCollision(player);

}

void LuckyBlock::dropItem()
{
    if (nIsEmpty || nStateBounce != 0 || nIsDropping)
        return;
    nIsDropping = true;
}