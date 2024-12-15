#pragma once
#include "World/World.hpp"
#include "Player.hpp"
#include "StateStack/State.hpp"
#include "StateStack/StateStack.hpp"
#include "Holder/ResourceHolder.hpp"
#include "Holder/ResourceIdentifiers.hpp"
#include "Music/MusicPlayer.hpp"
#include "Music/SoundPlayer.hpp"


#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game : private sf::NonCopyable
{
    public:

        Game();
        void run();

    private:

        void processInput();
        void update(sf::Time elapsedTime);
        void render();

        void updateStatistics(sf::Time elapsedTime);
        void registerStates();

    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow nWindow;
        TextureHolder nTextures;
        FontHolder nFonts;
        StateStack nGameStateStack;

        sf::Text nStatisticsText;
        sf::Time nStatisticsUpdateTime;
        World nWorld;
		Player nPlayer;
        MusicPlayer nMusicPlayer;
        SoundPlayer nSoundPlayer;
};