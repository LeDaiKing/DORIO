#include "Animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


Animation::Animation()
: nSprite()
, nFrameSize()
, nElapsedTime(sf::Time::Zero)
, nAnimationID(0)
, nFlipped(false)
{
}

Animation::Animation(const sf::Texture& texture)
: nSprite(texture)
, nFrameSize()
, nElapsedTime(sf::Time::Zero)
, nAnimationID(0)
, nFlipped(false)
{
}

void Animation::setTexture(const sf::Texture& texture)
{
	nSprite.setTexture(texture);
}

const sf::Texture* Animation::getTexture() const
{
	return nSprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	nFrameSize = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return nFrameSize;
}

void Animation::setFlipped(bool flag)
{
	nFlipped = flag;
}

bool Animation::isFlipped() const
{
	return nFlipped;
}

void Animation::addTypeAnimation(std::size_t numFrames, sf::Time duration, bool repeat)
{
	nTypeAnimations.push_back(AnimationType(numFrames, duration, repeat));
}

void Animation::setAnimationID(std::size_t type)
{
	nAnimationID = type;
}
// void Animation::setNumFrames(std::size_t numFrames)
// {
// 	nNumFrames = numFrames;
// }

// std::size_t Animation::getNumFrames() const
// {
// 	return nNumFrames;
// }

// void Animation::setDuration(sf::Time duration)
// {
// 	nDuration = duration;
// }

// sf::Time Animation::getDuration() const
// {
// 	return nDuration;
// }

void Animation::setRepeating(bool flag)
{
	nTypeAnimations[nAnimationID].nRepeat = flag;
}

bool Animation::isRepeating() const
{
	return nTypeAnimations[nAnimationID].nRepeat;
}

void Animation::restart()
{
	nTypeAnimations[nAnimationID].nCurrentFrame = 0;
}

bool Animation::isFinished() const
{
	return nTypeAnimations[nAnimationID].nCurrentFrame >= nTypeAnimations[nAnimationID].nNumFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::update(sf::Time dt)
{
	std::size_t nNumFrames = nTypeAnimations[nAnimationID].nNumFrames;
	sf::Time nDuration = nTypeAnimations[nAnimationID].nDuration;
	std::size_t &nCurrentFrame = nTypeAnimations[nAnimationID].nCurrentFrame;

	sf::Time timePerFrame = nDuration / static_cast<float>(nNumFrames);
	nElapsedTime += dt;

	// sf::Vector2i textureBounds(nSprite.getTexture()->getSize());
	sf::IntRect textureRect = nSprite.getTextureRect();

	textureRect = sf::IntRect(nCurrentFrame * nFrameSize.x, nAnimationID * nFrameSize.y, nFrameSize.x, nFrameSize.y);

	
	// While we have a frame to process
	while (nElapsedTime >= timePerFrame && (nCurrentFrame <= nNumFrames || nTypeAnimations[nAnimationID].nRepeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture

		// And progress to next frame
		nElapsedTime -= timePerFrame;
		if (nTypeAnimations[nAnimationID].nRepeat)
		{
			nCurrentFrame = (nCurrentFrame + 1) % nNumFrames;

			if (nCurrentFrame == 0)
				textureRect = sf::IntRect(0, nAnimationID *  nFrameSize.y, nFrameSize.x, nFrameSize.y);
		}
		else
		{
			nCurrentFrame++;
		}
	}

	if (nFlipped)
	{
		textureRect.left = textureRect.left + textureRect.width;
		textureRect.width = -textureRect.width;
	}
	nSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(nSprite, states);
}