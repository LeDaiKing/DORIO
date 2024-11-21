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
, Entity(textures.get(toTextureID(type)))
{
	addAnimationState(State::Idle, 3, 11, sf::seconds(1.f), true);
	addAnimationState(State::Walk, 5, 12, sf::seconds(0.6f), true);
	addAnimationState(State::Jump, 4, 1, sf::seconds(0.6f));
	addAnimationState(State::DoubleJump, 0, 6, sf::seconds(0.2f));

	setAnimationState(State::Idle);

	setUpEntity();
}

// void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
// {
// }

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
	Entity::updateCurrent(dt);
	if (nOnGround) stateJump = 0;
}

void Dough::setUpEntity()
{
	nSpeed = sf::Vector2f(1024.f, 128.f);
	nMaxVelocity = sf::Vector2f(200.f, 1024.f);
	friction = sf::Vector2f(0.f, 0.f);
	nJumpVelocitty = 250;
}

void Dough::jump()
{
	if (stateJump == 0)
	{
		Entity::jump();
		stateJump++;
	} 
	else if (stateJump == 1)
	{
		setAnimationState(State::DoubleJump);
		addVelocity(0.f, -getVelocity().y);
		addVelocity(0.f, -nJumpVelocitty);
		stateJump++;
	} 
}