#pragma once
#include "Category.hpp"
#include "SpriteNode.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

// class Dough;
class Entity;

class Block : public SpriteNode
{

    public:
        enum Type
        {
            Floor1,
            UnderFloor1,
            StaticBlock1,
            Breakable1,
            LuckyBlock1,
            Gold,

            Floor2,
            UnderFloor2,
            StaticBlock2,
            Breakable2,
            LuckyBlock2,

            SlideBlock,
            JumpyBlock,

            SewerPipe,
        };

    public:

        Block(Type type, sf::Vector2f position);
        virtual unsigned int getCategory() const;

        virtual void applyNormal(SceneNode& graph);
        virtual void handleBottomCollision(Entity& player) = 0;
        virtual void handleTopCollision(Entity& entity) = 0;

        virtual sf::FloatRect getBoundingRect() const;
        virtual bool isBroken() const;
        virtual void load(std::ifstream& file);
        virtual void save(std::ofstream& file);
        Type getType() const;
    private:
        Type nType;
        // virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        // virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};