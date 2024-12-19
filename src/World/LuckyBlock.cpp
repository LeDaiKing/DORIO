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

Textures::ID toTextureIDEmpty(LuckyBlock::Type type)
{
    switch (type)
    {
        case LuckyBlock::Type::LuckyBlock1:
            return Textures::StaticLuckyBlock1;
        case LuckyBlock::Type::LuckyBlock2:
            return Textures::StaticLuckyBlock2;
        default:
            break;
    }
    return Textures::StaticLuckyBlock1;
}

LuckyBlock::LuckyBlock(Type type, sf::Vector2f position)
: BouncingBlock(type, position)
, nIsEmpty(true)
, nIsDropping(false)
, nTexture(TextureHolder::getInstance().get(toTextureIDEmpty(type)))
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
        ItemPair item;
        item.first = nItems.back();
        item.second = ItemFactory::createAppearAnimation(nItems.back());
        nQueueItems.push_back(std::move(item));
        nQueueItems.back().second->setPosition(0, -16);
        nItems.pop_back();
        nIsDropping = false;
    }
    if (!(int)nItems.size() && nStateBounce == 0)
    {
        nIsEmpty = true;
        nSprite.setTexture(nTexture);
    }

    for (auto it = nQueueItems.begin(); it != nQueueItems.end();)
    {
        if ((*it).second->isFinished())
        {
            Command command;
            command.category = Category::ItemScene;
            command.action = [this, it, &commands] (SceneNode& graph, sf::Time)
            {
                if ((*it).first == Item::Coin)
                {
                    return;
                }
                std::unique_ptr<Item> item = ItemFactory::createItem((*it).first, getWorldPosition() + sf::Vector2f(0.f, -32.f));
                graph.attachChild(std::move(item));
            };
            commands.push(command);
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
    nItems.push_back(type);    
}

void LuckyBlock::handleBottomCollision(Entity& player)
{
    if (player.getCategory() & Category::PlayerDough)
    {
        Dough& dough = static_cast<Dough&>(player);
        dropItem();
        if (nIsEmpty)
        {
            sf::FloatRect bound = getBoundingRect();
            dough.move(0.f, bound.top + bound.height - dough.getBoundingRect().top);
            dough.setVelocity(dough.getVelocity().x, 0);
            return;
        }
        if ((int)nItems.size() && nItems.back() == Item::Coin)
        {
            dough.addCoins(1);
        }
    }
    BouncingBlock::handleBottomCollision(player);

}

void LuckyBlock::dropItem()
{
    if (nIsEmpty || nStateBounce != 0 || nIsDropping)
        return;
    nIsDropping = true;
}

void LuckyBlock::randomItem(int num)
{
    if (num == -1) num = randomInt(5) + 1;
    nIsEmpty = false;
    bool isGrowUp = false;
    while(num--)
    {
        Item::Type type = static_cast<Item::Type>(randomInt(4));
        if (isGrowUp)
        {
            type = Item::Coin;
        }
        else
        {
            if (type != Item::Coin)
            {
                isGrowUp = true;
            }
        }

        addItem(type);
    }
}

void LuckyBlock::save(std::ofstream& file)
{
    Block::save(file);
    file.write(reinterpret_cast<const char*>(&nIsEmpty), sizeof(nIsEmpty));
    int size = nItems.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (auto& item : nItems)
    {
        file.write(reinterpret_cast<const char*>(&item), sizeof(item));
    }
    size = nQueueItems.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (auto& item : nQueueItems)
    {
        file.write(reinterpret_cast<const char*>(&item.first), sizeof(item.first));
    }
}

void LuckyBlock::load(std::ifstream& file)
{
    Block::load(file);
    file.read(reinterpret_cast<char*>(&nIsEmpty), sizeof(nIsEmpty));
    int size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < size; ++i)
    {
        Item::Type type;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        nItems.push_back(type);
    }
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < size; ++i)
    {
        Item::Type type;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        ItemPair item;
        item.first = type;
        item.second = ItemFactory::createAppearAnimation(type);
        nQueueItems.push_back(std::move(item));
    }
}