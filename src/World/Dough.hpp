#pragma once
#include "Entity.hpp"
#include "Animation.hpp"

#include  "../Holder/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Dough : public Entity
{
	public:
		enum Type
		{
			normal,
		};

	public:
		Dough(Type type, const TextureHolder& textures);
		virtual unsigned int getCategory() const;


	private:
		virtual void updateCurrent(sf::Time dt);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type nType;
		Animation nSprite;
};