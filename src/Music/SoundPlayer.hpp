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
        ~SoundPlayer() = default;
        void play(SoundEffect::ID effect);
        void removeStoppedSounds();
        void setVolume(float volume);
        float getVolume() const;
    private:
        std::list<sf::Sound> mSounds;
        float nVolume;
};