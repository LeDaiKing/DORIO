#include "Game.hpp"
#include <iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: nWindow(sf::VideoMode(800, 600), "Input", sf::Style::Close)
, nWorld(nWindow)
, nPlayer()
, nTextures()
, nFonts()
, nGameStateStack(State::Context(nWindow, nTextures, nFonts, nPlayer))
, nStatisticsText()
, nStatisticsUpdateTime()
{
	nWindow.setKeyRepeatEnabled(false);
	nTextures.load(Textures::ID::TitleScreen, "res/Background/title.png");
	nFonts.load(Fonts::Main, "res/Fonts/Sansation.ttf");
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
}
