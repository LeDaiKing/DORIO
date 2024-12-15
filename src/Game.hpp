#pragma once
#include "World/World.hpp"
#include "Player.hpp"

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

        // void loadTextures();

        void updateStatistics(sf::Time elapsedTime);

    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow nWindow;
        
        World nWorld;
		Player nPlayer;

};

