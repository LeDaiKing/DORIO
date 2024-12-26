#include "MusicPlayer.hpp"
#include <stdexcept>

MusicPlayer::MusicPlayer()
: mMusic()
, mFilenames()
, mVolume(0.f)
{
    mFilenames[Music::ID::BackgroundTheme] = "res/Music/BackgroundTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
    std::string filename = mFilenames[theme];
    if (!mMusic.openFromFile(filename))
    {
        throw std::runtime_error("Music " + filename + " could not be loaded");
    }

    mMusic.setVolume(mVolume);
    mMusic.setLoop(true);
    mMusic.play();
}

void MusicPlayer::stop()
{
    mMusic.stop();
}

void MusicPlayer::setPaused(bool isPaused)
{
    if (isPaused) mMusic.pause();
    else mMusic.play();
}

void MusicPlayer::setVolume(float volume) {
    mVolume = volume;
    mMusic.setVolume(volume);
}

float MusicPlayer::getVolume() const {
    return mVolume;
}
