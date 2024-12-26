#pragma once
#include "Entity.hpp"
#include "../Utility.hpp"
#include  "../Holder/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Enemy;
class Block;
class SewerPipe;
class Dough : public Entity
{
	public:
		enum Type
		{
			Dough1,
			Dough2,
		};

		enum GrowUp
		{
			Small,
			Big,
			FireBig,
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
		void sit();
		void standUp();
		void preFire();
		virtual sf::FloatRect getBoundingRect() const;
		void growUPBig();
		void growUPFireBig();
		void addHitPoints(int points);
		void addCoins(int coins);
		int getCoinsCount() const;
		int getScore() const;
		void setCheckPoint(sf::Vector2f checkPoint);
		void resetCheckPoint();
		virtual void load(std::ifstream& file);
		virtual void save(std::ofstream& file);
		void setMotionless(bool flag);

	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void setUpEntity() ;
		void fire(CommandQueue& commands);
		virtual void setAnimationState(State type);

		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void toSmall();

	private:
		Type nType;
		GrowUp nGrowUp;
		int stateJump = 0;
		int nJumpVelocity2;
		sf::Time nTimeDamage = sf::Time::Zero;
		Animation nBig;
		Animation nFireBig;
		sf::Vector2f nBigHitBox;
		int nCoinsCount;
		sf::Vector2f nCheckPoint;
		int nScore;
		bool nMotionless;
		SewerPipe* nPipe;
};