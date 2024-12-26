#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer() 
: mSounds() {
    SoundBufferHolder::getInstance().load(SoundEffect::ID::ClickedButton, "res/Sound/ClickedButton.wav");
}


void SoundPlayer::play(SoundEffect::ID effect) {
  mSounds.push_back(sf::Sound(SoundBufferHolder::getInstance().get(effect)));
  mSounds.back().setVolume(nVolume);
  mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds() {
    mSounds.remove_if([](const sf::Sound& sound) {
        return sound.getStatus() == sf::Sound::Stopped;
    });
}

void SoundPlayer::setVolume(float volume) {
    nVolume = volume;
}

float SoundPlayer::getVolume() const {
    return nVolume;
}
