#pragma once
#include "Animation.hpp"
#include "SceneNode.hpp"
class Block;
class Entity : public SceneNode
{
	public:

		enum State
		{
			None,
			Idle,
			Walk,
			Jump,
			DoubleJump,
			Fall,
			Attack,
			Hit,
			Dead,
			StateCount
		};

		Entity(const sf::Texture& texture);
		// virtual ~Entity();

		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		void addVelocity(sf::Vector2f velocity);
		void addVelocity(float vx, float vy);
		void accelerate(sf::Vector2f velocity);
		void accelerate(float vx, float vy);
		// void applyFriction(sf::Vector2f friction);
		sf::Vector2f getVelocity() const;
		bool getDirection() const;

		virtual bool isMarkedForRemoval() const;
		// void remove();

		void setOnGround(bool flag);
		virtual void walk(bool nDirection);
		virtual void jump();

		void setAnimationState(State type);

		virtual sf::FloatRect getBoundingRect() const;
		virtual unsigned int getCategory() const;

		// virtual sf::FloatRect getHitBox() const;
		
		virtual void getDamage(int damage);
		// virtual void attack() = 0;

		void updateClosestTopBlock(Block* block);
		void updateClosestBottomBlock(Block* block);


	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

		void addAnimationState(State, std::size_t row, std::size_t numFrames, sf::Time duration, sf::Vector2i frameSize, bool repeat = false);
		virtual void setUpEntity() = 0;

	private:
		sf::Vector2f nVelocity;
		sf::Vector2f nAcceleration;

	protected:
		bool nDirection;
		Animation nSprite;
		State nCurrentState;
		sf::Vector2f nHitBox;
		sf::Vector2f nSpeed;
		sf::Vector2f nMaxVelocity;
		// sf::Vector2f nFriction;
		int nJumpVelocity;
		bool nOnGround;
		int nHitPoints = 1;
		Block* nClosestTopBlock = nullptr;
		Block* nClosestBottomBlock = nullptr;
};