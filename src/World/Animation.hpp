#pragma once

#include<unordered_map>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <unordered_map>

struct AnimationState
{
	std::size_t nRow;
	std::size_t nNumFrames;
	sf::Time nDuration;
	sf::Vector2i nFrameSize;
	bool nRepeat;

	AnimationState(std::size_t row, std::size_t numFrames, sf::Time duration, sf::Vector2i frameSize, bool repeat = false)
	: nRow(row), nNumFrames(numFrames), nDuration(duration), nFrameSize(frameSize), nRepeat(repeat)
	{
	}
};


class Animation : public sf::Drawable, public sf::Transformable
{
	public:
		explicit Animation(const sf::Texture& texture);
		Animation();
		// deconstructor
		~Animation();

		sf::Vector2i getFrameSize() const;

		// void setNumFrames(std::size_t numFrames);
		// std::size_t getNumFrames() const;

		// void setDuration(sf::Time duration);
		// sf::Time getDuration() const;

		// void setPosition(float x, float y);

		void setFlipped(bool flag);
		bool isFlipped() const;

		void turnInverse();

		void addAnimationState(int ID, std::size_t row, std::size_t numFrames, sf::Time duration, sf::Vector2i frameSize, bool repeat = false);

		void setAnimationState(int ID);

		int getCurrentAnimationID() const;

		bool isFinished() const;

		sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;

		void update(sf::Time dt);


	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		sf::Sprite nSprite;
		std::size_t nCurrentFrame;
		AnimationState* nCurrentAnimation;
		std::unordered_map<int, AnimationState*> nAnimations;
		sf::Time nElapsedTime;
		bool nFlipped;
		bool nInverse;
};
