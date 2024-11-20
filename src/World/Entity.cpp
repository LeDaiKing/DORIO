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
	if (nVelocity.x != 0.f && nVelocity.y != 0.f)
	{
		nVelocity.x = nVelocity.x / std::sqrt(2.f);
		nVelocity.y = nVelocity.y / std::sqrt(2.f);
	}

	move(nVelocity * dt.asSeconds());
	
	nVelocity = sf::Vector2f(0.f, 0.f);
}