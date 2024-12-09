#pragma once
#include "Entity.hpp"
#include "../Utility.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Enemy;
class Block;
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

		virtual void jump();
		virtual void attackEnemy(Enemy& enemy);
		virtual void getDamage(int damage);

		void handleCollisionEnemies(SceneNode& graph);
		void handleCollisionItems(SceneNode& graph);
		int getStateJump() const;
		void setStateJump(int state);

	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void setUpEntity() ;

		// virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type nType;
		int stateJump = 0;
		int nJumpVelocity2;
		sf::Time nTimeDamage = sf::Time::Zero;
};