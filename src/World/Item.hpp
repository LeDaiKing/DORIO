#pragma once
#include "Category.hpp"
#include "SceneNode.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "Animation.hpp"

class Dough;
class Item : public SceneNode
{
    public:
        enum Type
        {
            Coin,
            Heart,
        };
    public:
        Item(Type type, sf::Vector2i frameSize, std::size_t numFrames = 1, sf::Time duration = sf::Time::Zero);
        virtual unsigned int getCategory() const;
        virtual sf::FloatRect getBoundingRect() const;
        virtual void collectedBy(Dough& player) = 0;
        virtual bool isMarkedForRemoval() const;
    protected:
        virtual void updateCurrent(sf::Time dt);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    protected:
        Type nType;
        bool nIsCollected;
    private:
        Animation nAnimation;
};