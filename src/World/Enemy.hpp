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
			Chicken,
			Snail,
			SnailShell,
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

        virtual void attackPlayer(Dough& player) ;
		Type getType();
		// virtual void getDamage(int damage) = 0;
		virtual void addMoveBehavior(sf::Vector2f offset);
		virtual void addWaitBehavior(sf::Time time);
		virtual void addTurnBehavior();
		void addBehavior(int type, float x, float y);
		virtual void isTargetInRange(const sf::Vector2f& target);
		virtual sf::FloatRect getBoundingRect() const;
		virtual bool isAutoAI();
		virtual void load(std::ifstream& file);
		virtual void save(std::ofstream& file);
	protected:
		void setAIState(AIState state);
		virtual void moveToPosition(sf::Time dt);
		virtual void wait(sf::Time time);
		virtual void turn();
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void autoMove();
		// virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
	private:

	private:
		Type nType;
	protected:
		std::vector<std::function<void()>> nBehaviors;
		std::vector<std::pair<int, std::pair<float, float>>> nBeList;
		int nCurBehavior;
		int nDirLoop;
		AIState nAIState;
		sf::Time nWaitingTime;
		sf::Vector2f nDestination;
};