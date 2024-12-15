#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <list>
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"


class SoundPlayer : private sf::NonCopyable
{
    public:
        SoundPlayer();
        void play(SoundEffect::ID effect);
        void removeStoppedSounds();
    private:
        SoundBufferHolder mSoundBuffers;
        std::list<sf::Sound> mSounds;
};