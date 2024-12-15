#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer() 
: mSoundBuffers()
, mSounds() {
    mSoundBuffers.load(SoundEffect::ID::ClickedButton, "res/Sound/ClickedButton.wav");
}

void SoundPlayer::play(SoundEffect::ID effect) {
    mSounds.push_back(sf::Sound(mSoundBuffers.get(effect)));
    mSounds.back().play();
}



void SoundPlayer::removeStoppedSounds() {
    mSounds.remove_if([](const sf::Sound& sound) {
        return sound.getStatus() == sf::Sound::Stopped;
    });
}