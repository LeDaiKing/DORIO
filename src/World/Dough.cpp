#include "Dough.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Category.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Textures::ID toTextureID(Dough::Type type)
{
	switch (type)
	{
		case Dough::normal:
			return Textures::normal;

	}
	return Textures::normal;
}

Dough::Dough(Type type, const TextureHolder& textures)
: nType(type)
, nSprite(textures.get(toTextureID(type)))
{
	centerOrigin(nSprite);

	nSprite.setFrameSize(sf::Vector2i(48, 48));
	nSprite.setNumFrames(12);
	nSprite.setDuration(sf::seconds(1));
	nSprite.setRepeating(true);
}

void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite, states);
}

unsigned int Dough::getCategory() const
{
	switch (nType)
	{
		case normal:
			return Category::PlayerDough;

		default:
			return Category::PlayerDough;
	}
}

void Dough::updateCurrent(sf::Time dt)
{
	nSprite.update(dt);
	Entity::updateCurrent(dt);
}