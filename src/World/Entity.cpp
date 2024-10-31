#include "Entity.hpp"


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

void Entity::accelerate(sf::Vector2f velocity)
{
	nVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	nVelocity.x += vx;
	nVelocity.y += vy;
}

void Entity::updateCurrent(sf::Time dt)
{	
	move(nVelocity * dt.asSeconds());
}