#include "Game.hpp"
#include <iostream>
#include "StateStack/TitleState.hpp"
#include "StateStack/GameState.hpp"
#include "StateStack/PauseState.hpp"
#include "StateStack/MenuState.hpp"
#include "StateStack/ChooseModeState.hpp"
#include "StateStack/ChoosePlayerState.hpp"


#include "Holder/ResourceHolder.hpp"
const int numFrames = 60;

const sf::Time Game::TimePerFrame = sf::seconds(1.f/numFrames);

Game::Game()
: nWindow(sf::VideoMode(1200, 800), "Input", sf::Style::Close)
, nPlayer()
, nMusicPlayer()
, nSoundPlayer()
, nGameStateStack(State::Context(nWindow, nPlayer, nMusicPlayer, nSoundPlayer))
, nStatisticsText()
, nStatisticsUpdateTime()
{
	nWindow.setKeyRepeatEnabled(false);
	loadTextures();
	// FontHolder::getInstance().load(Fonts::Mario, "res/Fonts/SuperMario256.ttf")
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

			if (nGameStateStack.isEmpty())
				nWindow.close();
		}
		updateStatistics(elapsedTime);
		render();
	}
}

void Game::loadTextures() {
	TextureHolder::getInstance().load(Textures::ID::TitleScreen, "res/Background/title.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseModeScreen, "res/Background/background.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonNormal, "res/UI/SmallRectButton.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonSelected, "res/UI/SmallRectButton.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonPressed, "res/UI/SmallRectButton.png");
	TextureHolder::getInstance().load(Textures::ID::SquareButtonNormal, "res/UI/SquareButton1111.png");
	TextureHolder::getInstance().load(Textures::ID::SquareButtonSelected, "res/UI/SquareButton222.png");
	TextureHolder::getInstance().load(Textures::ID::SquareButtonPressed, "res/UI/SquareButton333.png");
	TextureHolder::getInstance().load(Textures::ID::CreativeButtonNormal, "res/UI/CreativeButton111.png");
	TextureHolder::getInstance().load(Textures::ID::CreativeButtonSelected, "res/UI/CreativeButton222.png");
	TextureHolder::getInstance().load(Textures::ID::CreativeButtonPressed, "res/UI/CreativeButton222.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonNormal, "res/UI/BackButton3.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonSelected, "res/UI/BackButton3.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonPressed, "res/UI/BackButton3.png");
	TextureHolder::getInstance().load(Textures::Dirt, "res/Background/Dirt.png");
	TextureHolder::getInstance().load(Textures::Dough1, "res/Dough/dough.png");
	TextureHolder::getInstance().load(Textures::Dough2, "res/Dough/tile001.png");
	TextureHolder::getInstance().load(Textures::BigDough, "res/Dough/big_dough.png");
	TextureHolder::getInstance().load(Textures::FireBigDough, "res/Dough/fire_dough.png");
	TextureHolder::getInstance().load(Textures::Sky, "res/Background/bg_kitchen_930x608.png");
	TextureHolder::getInstance().load(Textures::Enemy, "res/Enemy/Enemy.png");
	TextureHolder::getInstance().load(Textures::Ghost, "res/Enemy/Ghost/Ghost.png");
	TextureHolder::getInstance().load(Textures::Chicken, "res/Enemy/Chicken/Chicken.png");
	TextureHolder::getInstance().load(Textures::CockRoach, "res/Enemy/CockRoach/CockRoach.png");
	TextureHolder::getInstance().load(Textures::Snail, "res/Enemy/Snail/Snail.png");
	TextureHolder::getInstance().load(Textures::SnailShell, "res/Enemy/Snail/SnailShell.png");
	TextureHolder::getInstance().load(Textures::Breakable, "res/Background/Breakable/Breakable.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation, "res/Background/Breakable/BreakAnimation.png");
	TextureHolder::getInstance().load(Textures::LuckyBlock, "res/Background/LuckyBlock/LuckyBlock.png");
	TextureHolder::getInstance().load(Textures::StaticLuckyBlock, "res/Background/LuckyBlock/StaticLuckyBlock.png");
	TextureHolder::getInstance().load(Textures::JumpyBlock, "res/Background/JumpyBlock/JumpyBlock.png");
	TextureHolder::getInstance().load(Textures::Coin, "res/Item/Coin.png");
	TextureHolder::getInstance().load(Textures::CoinAnimation, "res/Item/CoinAnimation.png");
	TextureHolder::getInstance().load(Textures::Heart, "res/Item/Heart.png");
	TextureHolder::getInstance().load(Textures::HeartAnimation, "res/Item/StarAnimation.png");
	TextureHolder::getInstance().load(Textures::FireBall, "res/Item/Fireball.png");
	TextureHolder::getInstance().load(Textures::HeartIcon, "res/Hub/HeartIcon.png");
	TextureHolder::getInstance().load(Textures::CoinIcon, "res/Hub/CoinIcon.png");
	TextureHolder::getInstance().load(Textures::ClockIcon, "res/Hub/ClockIcon.png");
	TextureHolder::getInstance().load(Textures::SettingBut, "res/Buttons/Settings.png");
	TextureHolder::getInstance().load(Textures::WhiteText, "res/Text/WhiteText.png");
	TextureHolder::getInstance().load(Textures::BlackText, "res/Text/BlackText.png");
	FontHolder::getInstance().load(Fonts::Main, "res/Fonts/SuperMario256.ttf");
}

void Game::processInput() {
  sf::Event event;
  while (nWindow.pollEvent(event)) {
    nGameStateStack.handleEvent(event);

    if (event.type == sf::Event::Closed) nWindow.close();
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
