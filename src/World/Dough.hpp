#pragma once
#include "Entity.hpp"
#include "../Utility.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Enemy;

class Dough : public Entity
{
	public:
		enum Type
		{
			Dough1,
			Dough2,
		};

		using Entity::State;

	public:
		Dough(Type type);
		virtual unsigned int getCategory() const;

		virtual void setUpEntity();
		virtual void jump();
		virtual void attackEnemy(Enemy& enemy);
		virtual void getDamage();

		void handleCollisionEnemies(SceneNode& graph);
		// void handleCollisionItems(SceneNode& graph);
	private:
		virtual void updateCurrent(sf::Time dt);
		// virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type nType;
		int stateJump = 0;
		sf::Time nTimeDamage = sf::Time::Zero;
};