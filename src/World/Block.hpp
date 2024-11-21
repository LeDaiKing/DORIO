#pragma once
#include "Category.hpp"
#include "SpriteNode.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Block : public SpriteNode
{

    public:
        Block(const sf::Texture& texture, sf::Vector2f position);
        virtual unsigned int getCategory() const;

        void applyNormal(SceneNode& graph);

        virtual sf::FloatRect getBoundingRect() const;

    private:
        // virtual void updateCurrent(sf::Time dt);
        // virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};