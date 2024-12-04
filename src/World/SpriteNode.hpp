#pragma once

#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode
{
	public:
		explicit SpriteNode(const sf::Texture& texture);
		SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
		
	
	protected:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	protected:
		sf::Sprite nSprite;
};
