#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

struct AnimationType
{
	std::size_t nCurrentFrame;
	std::size_t nNumFrames;
	sf::Time nDuration;
	bool nRepeat;

	AnimationType(std::size_t nNumFrames, sf::Time nDuration, bool nRepeat = false)
	: nCurrentFrame(0), nNumFrames(nNumFrames), nDuration(nDuration), nRepeat(nRepeat)
	{
	}
};


class Animation : public sf::Drawable, public sf::Transformable
{
	public:
		Animation();
		explicit Animation(const sf::Texture& texture);

		void setTexture(const sf::Texture& texture);
		const sf::Texture* getTexture() const;

		void setFrameSize(sf::Vector2i nFrameSize);
		sf::Vector2i getFrameSize() const;

		// void setNumFrames(std::size_t numFrames);
		// std::size_t getNumFrames() const;

		// void setDuration(sf::Time duration);
		// sf::Time getDuration() const;

		void setRepeating(bool flag);
		bool isRepeating() const;

		void setFlipped(bool flag);
		bool isFlipped() const;

		void addTypeAnimation(std::size_t numFrames, sf::Time duration, bool repeat = false);

		void setAnimationID(std::size_t type);

		void restart();
		bool isFinished() const;

		sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;

		void update(sf::Time dt);


	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		sf::Sprite nSprite;
		sf::Vector2i nFrameSize;
		std::size_t nAnimationID;
		std::vector<AnimationType> nTypeAnimations;
		sf::Time nElapsedTime;
		bool nFlipped;
};
