#pragma once
#include "Entity.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"
#include <functional>
#include <SFML/Graphics/Sprite.hpp>

class Dough;

class Enemy : public Entity
{
	public:
		enum Type
		{
			CockRoach,
			Ghost,
		};

		enum AIState
		{
			None,
			Patrolling,
			Turning,
			Waiting,
			Chasing,
			Firing,
		};

	public:
		// Enemy();
        Enemy(Type type, sf::Vector2f position);
		virtual unsigned int getCategory() const;

        virtual void attackPlayer(Dough& player) = 0;
		Type getType();
		// virtual void getDamage(int damage) = 0;
		virtual void addMoveBehavior(sf::Vector2f offset);
		virtual void addWaitBehavior(sf::Time time);
		virtual void addTurnBehavior();
		virtual void isTargetInRange(sf::Vector2f target);
	protected:
		void setAIState(AIState state);
		virtual void moveToPosition(sf::Time dt);
		virtual void wait(sf::Time time);
		virtual void turn();
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		// virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
	private:
		

	private:
		Type nType;
	protected:
		std::vector<std::function<void()>> nBehaviors;
		int nCurBehavior;
		int nDirLoop;
		AIState nAIState;
		sf::Time nWaitingTime;
		sf::Vector2f nDestination;
};