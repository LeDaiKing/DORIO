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
			Attack,
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
		virtual void Move(bool nDirection);
		virtual void jump();
		void setAnimationState(State type);


	protected:
		virtual void updateCurrent(sf::Time dt);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

		void addAnimationState(State, std::size_t row, std::size_t numFrames, sf::Time duration, bool repeat = false);

	private:
		sf::Vector2f nVelocity;
		sf::Vector2f nAcceleration;
		Animation nSprite;
		State nCurrentState;
		bool nDirection;

	protected:
		sf::Vector2f nSpeed;
		sf::Vector2f nMaxVelocity;
		sf::Vector2f friction;
		int nJumpVelocitty;
		int nJumpDistance;
};

