#pragma once
#include "Animation.hpp"
#include "SceneNode.hpp"

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

		virtual void setUpEntity() = 0;

		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		void addVelocity(sf::Vector2f velocity);
		void addVelocity(float vx, float vy);
		void accelerate(sf::Vector2f velocity);
		void accelerate(float vx, float vy);

		sf::Vector2f getVelocity() const;
		bool getDirection() const;

		void setOnGround(bool flag);
		virtual void walk(bool nDirection);
		virtual void jump();

		void setAnimationState(State type);

		virtual sf::FloatRect getBoundingRect() const;
		virtual unsigned int getCategory() const;

		virtual sf::FloatRect getHitBox() const;

		// virtual void attack() = 0;


	protected:
		virtual void updateCurrent(sf::Time dt);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

		void addAnimationState(State, std::size_t row, std::size_t numFrames, sf::Time duration, sf::Vector2i frameSize, bool repeat = false);

	private:
		sf::Vector2f nVelocity;
		sf::Vector2f nAcceleration;
		Animation nSprite;
		bool nDirection;

	protected:
		State nCurrentState;
		sf::Vector2f nHitBox;
		sf::Vector2f nSpeed;
		sf::Vector2f nMaxVelocity;
		sf::Vector2f friction;
		int nJumpVelocitty;
		bool nOnGround;
};

