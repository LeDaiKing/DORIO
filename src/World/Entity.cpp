#include "Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Entity::Entity(const sf::Texture& texture)
: nSprite(texture, sf::Vector2i(32, 32)), nCurrentState(State::None)
{
	centerOrigin(nSprite);
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	nVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	nVelocity.x = vx;
	nVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return nVelocity;
}

void Entity::addVelocity(sf::Vector2f velocity)
{
	nVelocity += velocity;
}

void Entity::addVelocity(float vx, float vy)
{
	nVelocity.x += vx;
	nVelocity.y += vy;
}

void Entity::accelerate(sf::Vector2f acceleration)
{
	nAcceleration += acceleration;
}

void Entity::accelerate(float vx, float vy)
{
	nAcceleration.x += vx;
	nAcceleration.y += vy;
}

void Entity::updateCurrent(sf::Time dt)
{
	

	addVelocity(nAcceleration * dt.asSeconds());
	move(nVelocity * dt.asSeconds());
	nAcceleration = sf::Vector2f(0.f, 0.f);
	
	//friction
	sf::Vector2f friction = sf::Vector2f(512.f, 64.f) * 0.8f * dt.asSeconds();

	if (nVelocity.x > 0)
	{
		nVelocity.x -= friction.x;
		if (nVelocity.x < 0)
			nVelocity.x = 0;
	}
	else if (nVelocity.x < 0)
	{
		nVelocity.x += friction.x;
		if (nVelocity.x > 0)
			nVelocity.x = 0;
	}

	if (nVelocity.y > 0)
	{
		nVelocity.y -= friction.y;
		if (nVelocity.y < 0)
			nVelocity.y = 0;
	}
	else if (nVelocity.y < 0)
	{
		nVelocity.y += friction.y;
		if (nVelocity.y > 0)
			nVelocity.y = 0;
	}

	if (nVelocity == sf::Vector2f(0.f, 0.f))
		setAnimationState(State::Idle);

	nSprite.update(dt);
}

void Entity::setAnimationState(State type)
{
	if (nCurrentState == type)
		return;

	nCurrentState = type;
	nSprite.setAnimationState(type);
}

void Entity::Move(bool nDirection)
{
	if (this -> nDirection != nDirection)
	{
		this -> nDirection = nDirection;
		nSprite.setFlipped(nDirection);
	}
	if (nCurrentState != State::Jump)
		setAnimationState(State::Walk);
	
	if (nDirection)
	{
		accelerate(-nSpeed.x, 0.f);
	}
	else
	{
		accelerate(nSpeed.x, 0.f);
	}
}

void Entity::jump()
{
	setAnimationState(State::Jump);
	addVelocity(0.f, -nJumpVelocitty);
}

void Entity::addAnimationState(State state, std::size_t row, std::size_t numFrames, sf::Time duration, bool repeat)
{
	nSprite.addAnimationState(state, row, numFrames, duration, repeat);
}


void Entity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(nSprite, states);
}

