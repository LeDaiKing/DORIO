#include "Animation.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


Animation::Animation()
: nSprite()
, nFrameSize()
, nNumFrames(0)
, nCurrentFrame(0)
, nDuration(sf::Time::Zero)
, nElapsedTime(sf::Time::Zero)
, nRepeat(false)
{
}

Animation::Animation(const sf::Texture& texture)
: nSprite(texture)
, nFrameSize()
, nNumFrames(0)
, nCurrentFrame(0)
, nDuration(sf::Time::Zero)
, nElapsedTime(sf::Time::Zero)
, nRepeat(false)
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

void Animation::setNumFrames(std::size_t numFrames)
{
	nNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return nNumFrames;
}

void Animation::setDuration(sf::Time duration)
{
	nDuration = duration;
}

sf::Time Animation::getDuration() const
{
	return nDuration;
}

void Animation::setRepeating(bool flag)
{
	nRepeat = flag;
}

bool Animation::isRepeating() const
{
	return nRepeat;
}

void Animation::restart()
{
	nCurrentFrame = 0;
}

bool Animation::isFinished() const
{
	return nCurrentFrame >= nNumFrames;
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
	sf::Time timePerFrame = nDuration / static_cast<float>(nNumFrames);
	nElapsedTime += dt;

	sf::Vector2i textureBounds(nSprite.getTexture()->getSize());
	sf::IntRect textureRect = nSprite.getTextureRect();

	if (nCurrentFrame == 0)
		textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);
	
	// While we have a frame to process
	while (nElapsedTime >= timePerFrame && (nCurrentFrame <= nNumFrames || nRepeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			// Move it down one line
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		// And progress to next frame
		nElapsedTime -= timePerFrame;
		if (nRepeat)
		{
			nCurrentFrame = (nCurrentFrame + 1) % nNumFrames;

			if (nCurrentFrame == 0)
				textureRect = sf::IntRect(0, 0, nFrameSize.x, nFrameSize.y);
		}
		else
		{
			nCurrentFrame++;
		}
	}

	nSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(nSprite, states);
}