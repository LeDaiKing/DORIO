#include "Animation.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

Animation::Animation(const sf::Texture& texture)
: nSprite(texture)
, nElapsedTime(sf::Time::Zero)
, nCurrentFrame(0)
, nCurrentAnimation(nullptr)
, nFlipped(false)
{
}

void Animation::setFlipped(bool flag)
{
	nFlipped = flag;
}

bool Animation::isFlipped() const
{
	return nFlipped;
}

void Animation::addAnimationState(int ID, std::size_t row, std::size_t numFrames, sf::Time duration, sf::Vector2i frameSize, bool repeat)
{
	nAnimations[ID] = new AnimationState(row, numFrames, duration, frameSize, repeat);
}


void Animation::setAnimationState(int ID)
{
	nCurrentAnimation = nAnimations[ID];
	nCurrentFrame = 0;
	nElapsedTime = sf::Time::Zero;
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

sf::Vector2i Animation::getFrameSize() const
{
	if (nCurrentAnimation == nullptr)
		return sf::Vector2i(32, 32);
	return nCurrentAnimation->nFrameSize;
}

bool Animation::isFinished() const
{
	if (nCurrentAnimation == nullptr)
		return false;

	return nCurrentFrame >= nCurrentAnimation->nNumFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(sf::Vector2f(0.f, 0.f), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::update(sf::Time dt)
{
	if (nCurrentAnimation == nullptr)
		return;

	std::size_t nNumFrames = nCurrentAnimation->nNumFrames;
	sf::Time nDuration = nCurrentAnimation->nDuration;
	std::size_t nRow = nCurrentAnimation->nRow;
	sf::Vector2i nFrameSize = nCurrentAnimation->nFrameSize;

	sf::Time timePerFrame = nDuration / static_cast<float>(nNumFrames);
	nElapsedTime += dt;

	// sf::Vector2i textureBounds(nSprite.getTexture()->getSize());
	sf::IntRect textureRect = nSprite.getTextureRect();

	textureRect = sf::IntRect(nCurrentFrame * nFrameSize.x, nRow * nFrameSize.y, nFrameSize.x, nFrameSize.y);

	
	// While we have a frame to process
	while (nElapsedTime >= timePerFrame && (nCurrentFrame < nNumFrames || nCurrentAnimation->nRepeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture

		// And progress to next frame
		nElapsedTime -= timePerFrame;
		if (nCurrentAnimation->nRepeat)
		{
			nCurrentFrame = (nCurrentFrame + 1) % nNumFrames;

			if (nCurrentFrame == 0)
				textureRect = sf::IntRect(0, nRow *  nFrameSize.y, nFrameSize.x, nFrameSize.y);
		}
		else
		{
			nCurrentFrame++;
		}
	}

	if (isFinished())
	{
		textureRect = sf::IntRect((nNumFrames - 1) * nFrameSize.x, nRow * nFrameSize.y, nFrameSize.x, nFrameSize.y);
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

Animation::~Animation()
{
	for (auto pair : nAnimations)
	{
		delete pair.second;
	}
}