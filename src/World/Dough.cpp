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
		case Dough::Dough1:
			return Textures::Dough1;
		case Dough::Dough2:
			return Textures::Dough2;

	}
	return Textures::Dough1;
}

Dough::Dough(Type type, const TextureHolder& textures)
: nType(type)
, Entity(textures.get(toTextureID(type)))
{
	switch (type)
	{
	case Dough1:
		addAnimationState(State::Idle, 48, 12, sf::seconds(1.f), sf::Vector2i(42, 42), true);
		addAnimationState(State::Walk, 164, 4, sf::seconds(0.5f), sf::Vector2i(48, 48), true);
		addAnimationState(State::Jump, 90, 14, sf::seconds(1.f), sf::Vector2i(58, 74), false);
		addAnimationState(State::DoubleJump, 90, 14, sf::seconds(0.6f), sf::Vector2i(58, 74), false);
		break;
	case Dough2:
		addAnimationState(State::Idle, 96, 11, sf::seconds(1.f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Walk, 160, 12, sf::seconds(0.6f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Jump, 128, 1, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
		addAnimationState(State::DoubleJump, 0, 6, sf::seconds(0.1f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Fall, 32, 1, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
		break;
	
	default:

		break;
	}
	

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
		case Dough1:
		case Dough2:
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
	switch (nType)
	{
	case Dough1:
	case Dough2:
		nHitBox = sf::Vector2f(25.f, 28.f);
		nSpeed = sf::Vector2f(1024.f, 128.f);
		nMaxVelocity = sf::Vector2f(200.f, 1024.f);
		friction = sf::Vector2f(0.f, 0.f);
		nJumpVelocitty = 270;
		break;
	
	default:
		break;
	}
	
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