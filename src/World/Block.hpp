#pragma once
#include "Category.hpp"
#include "SpriteNode.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Dough;
class Entity;

class Block : public SpriteNode
{

    public:
        enum Type
        {
            Dirt,
            Breakable,
            LuckyBlock,
            SlideBlock,
            JumpyBlock,
        };

    public:

        Block(Type type, sf::Vector2f position);
        virtual unsigned int getCategory() const;

        virtual void applyNormal(SceneNode& graph);
        virtual void handleBottomCollision(Dough& player) = 0;
        virtual void handleTopCollision(Entity& entity) = 0;

        virtual sf::FloatRect getBoundingRect() const;

    private:
        Type nType;
        // virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        // virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};