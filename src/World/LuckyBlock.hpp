#pragma once
#include "BouncingBlock.hpp"
#include "StaticBlock.hpp"
#include "Animation.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <stack>
#include "Item.hpp"
// #include "../Comman

class Entity;
class Item;
class LuckyBlock :  public BouncingBlock
{
    public:
        typedef std::pair<Command, std::unique_ptr<Animation>> ItemPair;
    public:
        LuckyBlock(Type type, sf::Vector2f position);
        virtual void handleBottomCollision(Entity& player);
        void dropItem();
        void addItem(Item::Type type);
        void randomItem(int num);

    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        

    private:
        bool nIsEmpty;
        sf::Texture nTexture;
        std::stack<ItemPair> nItems;
        bool nIsDropping;
        std::vector<ItemPair> nQueueItems;
};