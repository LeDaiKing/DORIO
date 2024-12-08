#pragma once
#include "Entity.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Dough;

class Enemy : public Entity
{
	public:
		Enemy();
        Enemy(sf::Vector2f position);
		virtual unsigned int getCategory() const;

		virtual void setUpEntity();
        virtual void attackPlayer(Dough& player);

		virtual void getDamage();

	private:
		// virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		// virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:

};