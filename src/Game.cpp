#include "Game.hpp"
#include <iostream>
#include "StateStack/TitleState.hpp"
#include "StateStack/GameState.hpp"
#include "StateStack/PauseState.hpp"
#include "StateStack/MenuState.hpp"
#include "StateStack/ChooseModeState.hpp"
#include "StateStack/ChoosePlayerState.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: nWindow(sf::VideoMode(800, 600), "Input", sf::Style::Close)
, nWorld(nWindow)
, nPlayer()
, nTextures()
, nFonts()
, nMusicPlayer()
, nSoundPlayer()
, nGameStateStack(State::Context(nWindow, nTextures, nFonts, nPlayer, nMusicPlayer, nSoundPlayer))
, nStatisticsText()
, nStatisticsUpdateTime()
{
	nWindow.setKeyRepeatEnabled(false);
	nTextures.load(Textures::ID::TitleScreen, "res/Background/title.png");
	nTextures.load(Textures::ID::ChooseModeScreen, "res/Background/sprout-valley.png");
	nTextures.load(Textures::ID::RectButtonNormal, "res/UI/SmallRectButton.png");
	nTextures.load(Textures::ID::RectButtonSelected, "res/UI/SmallRectButton.png");
	nTextures.load(Textures::ID::RectButtonPressed, "res/UI/SmallRectButton.png");
	nTextures.load(Textures::ID::SquareButtonNormal, "res/UI/SquareButton1.png");
	nTextures.load(Textures::ID::SquareButtonSelected, "res/UI/SquareButton2.png");
	nTextures.load(Textures::ID::SquareButtonPressed, "res/UI/SquareButton3.png");
	nTextures.load(Textures::ID::CreativeButtonNormal, "res/UI/CreativeButton11.png");
	nTextures.load(Textures::ID::CreativeButtonSelected, "res/UI/CreativeButton22.png");
	nTextures.load(Textures::ID::CreativeButtonPressed, "res/UI/CreativeButton22.png");
	nTextures.load(Textures::ID::BackButtonNormal, "res/UI/BackButton3.png");
	nTextures.load(Textures::ID::BackButtonSelected, "res/UI/BackButton3.png");
	nTextures.load(Textures::ID::BackButtonPressed, "res/UI/BackButton3.png");
	nFonts.load(Fonts::Main, "res/Fonts/SuperMario256.ttf");
	// nFonts.load(Fonts::Mario, "res/Fonts/SuperMario256.ttf")
	// mFont.loadFromFile("Media/Sansation.ttf");
	// mStatisticsText.setFont(mFont);
	// mStatisticsText.setPosition(5.f, 5.f);
	// mStatisticsText.setCharacterSize(10);

	registerStates();
	nGameStateStack.pushState(States::ID::Title);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	nWindow.setFramerateLimit(60);
	while (nWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (nGameStateStack.isEmpty())
				nWindow.close();
		}
		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processInput()
{
	sf::Event event;
	while (nWindow.pollEvent(event))
	{
		nGameStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			nWindow.close();
	}
}

void Game::update(sf::Time elapsedTime)
{
	nGameStateStack.update(elapsedTime);
}

void Game::render()
{
	nWindow.clear();	
	nGameStateStack.draw();
	nWindow.setView(nWindow.getDefaultView());
	// nWindow.draw(nStatisticsText);
	nWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	// mStatisticsUpdateTime += elapsedTime;
	// mStatisticsNumFrames += 1;

	// if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	// {
	// 	mStatisticsText.setString(
	// 		"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
	// 		"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
							 
	// 	mStatisticsUpdateTime -= sf::seconds(1.0f);
	// 	mStatisticsNumFrames = 0;
	// }
}

void Game::registerStates() {
	nGameStateStack.registerState<TitleState>(States::ID::Title);
	nGameStateStack.registerState<GameState>(States::ID::Game);
	nGameStateStack.registerState<PauseState>(States::ID::Pause);
	nGameStateStack.registerState<MenuState>(States::ID::Menu);
	nGameStateStack.registerState<ChooseModeState>(States::ID::ChooseMode);
	nGameStateStack.registerState<ChoosePlayerState>(States::ID::ChoosePlayer);
}
