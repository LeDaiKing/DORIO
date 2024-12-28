#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer() 
: mSounds() {
    SoundBufferHolder::getInstance().load(SoundEffect::ID::ClickedButton, "res/Sound/ClickedButton.wav");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::levelcomplete, "res/Sound/levelcomplete.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::achievement, "res/Sound/achievement.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::coin, "res/Sound/coin.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::gameover, "res/Sound/gameover.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::hurt, "res/Sound/hurt.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::item, "res/Sound/item.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::jump, "res/Sound/jump.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::land, "res/Sound/land.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::ui, "res/Sound/ui.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::powerup, "res/Sound/powerup.mp3");
    SoundBufferHolder::getInstance().load(SoundEffect::ID::shoot, "res/Sound/shoot.mp3");
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
