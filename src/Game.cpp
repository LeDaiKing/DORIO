#include "Game.hpp"
#include "Holder/ResourceHolder.hpp"
const int numFrames = 60;

const sf::Time Game::TimePerFrame = sf::seconds(1.f/numFrames);

Game::Game()
: nWindow(sf::VideoMode(1200, 800), "Input", sf::Style::Close)
, nWorld(nWindow)
, nPlayer()
{
	nWindow.setKeyRepeatEnabled(false);

	// mFont.loadFromFile("Media/Sansation.ttf");
	// mStatisticsText.setFont(mFont);
	// mStatisticsText.setPosition(5.f, 5.f);
	// mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	nWindow.setFramerateLimit(numFrames);
	while (nWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);
		}
		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processInput()
{
	CommandQueue& commands = nWorld.getCommandQueue();

	sf::Event event;
	while (nWindow.pollEvent(event))
	{
		nPlayer.handleEvent(event, commands);

		if (event.type == sf::Event::Closed)
			nWindow.close();
	}

	nPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time elapsedTime)
{
	nWorld.update(elapsedTime);
}

void Game::render()
{
	nWindow.clear();	
	nWorld.draw();

	nWindow.setView(nWindow.getDefaultView());
	// nWindow.draw(mStatisticsText);
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
