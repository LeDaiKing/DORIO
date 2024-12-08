#include "Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

Entity::Entity(const sf::Texture& texture)
: nSprite(texture), nCurrentState(State::None)
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
	nVelocity.x = std::max(nVelocity.x, -nMaxVelocity.x);
	nVelocity.x = std::min(nVelocity.x, nMaxVelocity.x);
	// nVelocity.y = std::max(nVelocity.y, -nMaxVelocity.y);
	// nVelocity.y = std::min(nVelocity.y, nMaxVelocity.y);
}

void Entity::addVelocity(float vx, float vy)
{
	nVelocity.x += vx;
	nVelocity.y += vy;
	nVelocity.x = std::max(nVelocity.x, -nMaxVelocity.x);
	nVelocity.x = std::min(nVelocity.x, nMaxVelocity.x);
	// nVelocity.y = std::max(nVelocity.y, -nMaxVelocity.y);
	// nVelocity.y = std::min(nVelocity.y, nMaxVelocity.y);
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

void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
{


	addVelocity(nAcceleration * dt.asSeconds());
	move(nVelocity * dt.asSeconds());
	nAcceleration = sf::Vector2f(0.f, 0.f);
	
	//friction
	sf::Vector2f friction = nSpeed * 0.8f * dt.asSeconds();

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

	// if ((nCurrentState == State::Jump || nCurrentState == State::DoubleJump)&& nVelocity.y == 0)
	// 	setAnimationState(State::Idle);

	// if (nVelocity.y != 0.f) nOnGround = false;s

	if (!nOnGround && nVelocity.y > 0 && nCurrentState != State::Hit && nCurrentState != State::Dead)
		setAnimationState(State::Fall);

	if (nVelocity.x == 0.f && nOnGround && nCurrentState != State::Hit && nCurrentState != State::Dead)
		setAnimationState(State::Idle);

	if (abs(nVelocity.y) > 10.f) nOnGround = false;
	// nOnGround = false;
	// if (nCurrentState == State::DoubleJump && nVelocity.y > 32)
	// 	setAnimationState(State::Jump);

	nSprite.update(dt);
}

void Entity::setAnimationState(State type)
{
	if (nCurrentState == type)
		return;

	nCurrentState = type;
	nSprite.setAnimationState(type);
}

void Entity::walk(bool nDirection)
{
	if (nOnGround && nCurrentState != State::Hit) 
		setAnimationState(State::Walk);

	// if (nCurrentState == State::Walk && this -> nDirection != nDirection)
	// 	return;
	if (this -> nDirection != nDirection)
	{
		this -> nDirection = nDirection;
		nSprite.setFlipped(nDirection);
	}
	
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
	if (!nOnGround)
		return;

	if (nCurrentState != State::Hit)
		setAnimationState(State::Jump);

	nOnGround = false;
	addVelocity(0.f, -nJumpVelocity);
}

void Entity::addAnimationState(State state, std::size_t row, std::size_t numFrames, sf::Time duration, sf::Vector2i frameSize, bool repeat)
{
	nSprite.addAnimationState(state, row, numFrames, duration, frameSize, repeat);
}


void Entity::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::FloatRect bounds = getBoundingRect();
	sf::RectangleShape rect(sf::Vector2f(bounds.width, bounds.height));
	rect.setPosition(bounds.left, bounds.top);
	rect.setFillColor(sf::Color(255, 255, 255, 0));
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(1.f);
	// target.draw(rect);
	target.draw(nSprite, states);
}

sf::FloatRect Entity::getBoundingRect() const
{
	// return getWorldTransform().transformRect(nSprite.getGlobalBounds());
	sf::FloatRect bound = getWorldTransform().transformRect(nSprite.getGlobalBounds());
	// sf::Vector2f pos = {bound.left + bound.width / 2 - nHitBox.x / 2, bound.top + bound.height - nHitBox.y};
	sf::Vector2f pos = getPosition() - sf::Vector2f(nHitBox.x / 2, nHitBox.y / 2);

	return sf::FloatRect(pos, nHitBox);
}

// sf::FloatRect Entity::getHitBox() const
// {
// 	sf::FloatRect bound = getWorldTransform().transformRect(nSprite.getGlobalBounds());
// 	// sf::Vector2f pos = {bound.left + bound.width / 2 - nHitBox.x / 2, bound.top + bound.height - nHitBox.y};
// 	sf::Vector2f pos = {bound.left + bound.width / 2 - nHitBox.x / 2, bound.top + bound.height / 2 - nHitBox.y / 2};

// 	return sf::FloatRect(pos, nHitBox);
// }

unsigned int Entity::getCategory() const
{
	return Category::Entity;
}

void Entity::setOnGround(bool flag)
{
	nOnGround = flag;
}

bool Entity::getDirection() const
{
	return nDirection;
}

bool Entity::isMarkedForRemoval() const
{
	return nCurrentState == State::Dead && nSprite.isFinished();
}

// void Entity::remove()
// {
// 	nParent->detachChild(*this);
// }

