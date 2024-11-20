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
	nSprite.addTypeAnimation(12, sf::seconds(1.f), true);
	nSprite.addTypeAnimation(12, sf::seconds(1.f), true);
	nSprite.addTypeAnimation(6, sf::seconds(0.8f), true);
	nSprite.addTypeAnimation(6, sf::seconds(0.8f), true);


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

void Dough::setAnimationID(std::size_t type)
{
	nSprite.setAnimationID(type);
}

void Dough::turnLeft()
{
	nSprite.setFlipped(true);
}

void Dough::turnRight()
{
	nSprite.setFlipped(false);
}