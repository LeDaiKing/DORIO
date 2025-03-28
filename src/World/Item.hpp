#pragma once
#include "Category.hpp"
#include "SceneNode.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "Animation.hpp"

class Entity;
class Item : public SceneNode
{
    public:
        enum Type
        {
            Coin,
            Heart,
            Big,
            FireBig,
            FireBall,
            Count,
        };
    public:
        Item(Type type, sf::Vector2f position);
        virtual unsigned int getCategory() const;
        virtual sf::FloatRect getBoundingRect() const;
        virtual Type getType() const;   
        virtual void activate(Entity& player) = 0;
        virtual bool isMarkedForRemoval() const;
        virtual void load(std::ifstream& file);
        virtual void save(std::ofstream& file);
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    protected:
        Type nType;
        bool nIsCollected;
        Animation nAnimation;
};