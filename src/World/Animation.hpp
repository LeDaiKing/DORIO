#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <unordered_map>

struct AnimationState
{
	std::size_t nRow;
	std::size_t nNumFrames;
	sf::Time nDuration;
	bool nRepeat;

	AnimationState(std::size_t nRow, std::size_t nNumFrames, sf::Time nDuration, bool nRepeat = false)
	: nRow(nRow), nNumFrames(nNumFrames), nDuration(nDuration), nRepeat(nRepeat)
	{
	}
};


class Animation : public sf::Drawable, public sf::Transformable
{
	public:
		explicit Animation(const sf::Texture& texture, sf::Vector2i frameSize);

		// deconstructor
		~Animation();

		sf::Vector2i getFrameSize() const;

		// void setNumFrames(std::size_t numFrames);
		// std::size_t getNumFrames() const;

		// void setDuration(sf::Time duration);
		// sf::Time getDuration() const;

		void setFlipped(bool flag);
		bool isFlipped() const;

		void addAnimationState(int ID, std::size_t row, ::size_t numFrames, sf::Time duration, bool repeat = false);

		void setAnimationState(int ID);

		bool isFinished() const;

		sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;

		void update(sf::Time dt);


	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		sf::Sprite nSprite;
		sf::Vector2i nFrameSize;
		std::size_t nCurrentFrame;
		AnimationState* nCurrentAnimation;
		std::unordered_map<int, AnimationState*> nAnimations;
		sf::Time nElapsedTime;
		bool nFlipped;
};
