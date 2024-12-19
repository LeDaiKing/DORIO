#include "Game.hpp"
#include <iostream>
#include "StateStack/TitleState.hpp"
#include "StateStack/GameState.hpp"
#include "StateStack/PauseState.hpp"
#include "StateStack/MenuState.hpp"
#include "StateStack/ChooseModeState.hpp"
#include "StateStack/ChoosePlayerState.hpp"
#include "StateStack/LoadingState.hpp"
#include "StateStack/ChooseCharState.hpp"

#include "Holder/ResourceHolder.hpp"
const int numFrames = 60;

const sf::Time Game::TimePerFrame = sf::seconds(1.f/numFrames);

Game::Game()
: //nWindow(sf::VideoMode(1200, 800), "Input", sf::Style::Close)
 nPlayer()
, nMusicPlayer()
, nSoundPlayer()
, nGameStateStack(State::Context(nWindow, nPlayer, nMusicPlayer, nSoundPlayer, nLoadingTask))
, nStatisticsText()
, nStatisticsUpdateTime()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	nWindow.create(sf::VideoMode(1200, 800, desktop.bitsPerPixel), "Game", sf::Style::Close);
	nWindow.setKeyRepeatEnabled(false);
	loadTextures();
	loadFonts();
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

	//Background
	TextureHolder::getInstance().load(Textures::ID::BackGround1, "res/Background/bg_kitchen.png");
	TextureHolder::getInstance().load(Textures::ID::BackGround2, "res/Background/bg_outside.png");
	TextureHolder::getInstance().load(Textures::ID::TitleScreen, "res/Background/title.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseCharScreen, "res/Background/ChooseCharScreen.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseModeScreen, "res/Background/ChooseModeScreen.png");

	//Button
	TextureHolder::getInstance().load(Textures::ID::RectButtonNormal, "res/UI/SmallRectButton.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonSelected, "res/UI/SmallRectButton.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonPressed, "res/UI/SmallRectButton.png");
	TextureHolder::getInstance().load(Textures::ID::SquareButtonNormal, "res/UI/SquareButton1111.png");
	TextureHolder::getInstance().load(Textures::ID::SquareButtonSelected, "res/UI/SquareButton222.png");
	TextureHolder::getInstance().load(Textures::ID::SquareButtonPressed, "res/UI/SquareButton333.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonNormal, "res/UI/BackButton.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonSelected, "res/UI/BackButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonPressed, "res/UI/BackButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::charSlot1Normal, "res/UI/charSlot.png");
	TextureHolder::getInstance().load(Textures::ID::charSlot1Pressed, "res/UI/charSlot_s.png");
	TextureHolder::getInstance().load(Textures::ID::charSlot1Selected, "res/UI/charSlot_s.png");
	TextureHolder::getInstance().load(Textures::ID::charSlot2Normal, "res/UI/char2Slot.png");
	TextureHolder::getInstance().load(Textures::ID::charSlot2Pressed, "res/UI/char2Slot_s.png");
	TextureHolder::getInstance().load(Textures::ID::charSlot2Selected, "res/UI/char2Slot_s.png");
	TextureHolder::getInstance().load(Textures::ID::previousButton, "res/UI/previousButton.png");
	TextureHolder::getInstance().load(Textures::ID::nextButton, "res/UI/nextButton.png");
    TextureHolder::getInstance().load(Textures::ID::char1Sprite, "res/UI/char1Sprite.png");
	TextureHolder::getInstance().load(Textures::ID::char2Sprite, "res/UI/char2Sprite.png");
	TextureHolder::getInstance().load(Textures::ID::saveButtonNormal, "res/UI/saveButton.png");
	TextureHolder::getInstance().load(Textures::ID::saveButtonSelected, "res/UI/saveButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::choosePlayerButtonNormal, "res/UI/choosePlayerButton.png");
	TextureHolder::getInstance().load(Textures::ID::choosePlayerButtonSelected, "res/UI/choosePlayerButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::chooseCharButtonNormal, "res/UI/chooseCharButton.png");
	TextureHolder::getInstance().load(Textures::ID::chooseCharButtonSelected, "res/UI/chooseCharButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::charIntro, "res/UI/charIntro.png");
	TextureHolder::getInstance().load(Textures::ID::charIntro2, "res/UI/charIntro2.png");
	TextureHolder::getInstance().load(Textures::ID::chooseModeButtonNormal, "res/UI/chooseModeButton.png");
	TextureHolder::getInstance().load(Textures::ID::chooseModeButtonSelected, "res/UI/chooseModeButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::kitchenModeNormal, "res/UI/kitchenMode.png");
	TextureHolder::getInstance().load(Textures::ID::kitchenModeSelected, "res/UI/kitchenMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::hallwayModeNormal, "res/UI/hallwayMode.png");
	TextureHolder::getInstance().load(Textures::ID::hallwayModeSelected, "res/UI/hallwayMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::gardenModeNormal, "res/UI/gardenMode.png");
	TextureHolder::getInstance().load(Textures::ID::gardenModeSelected, "res/UI/gardenMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::creativeModeNormal, "res/UI/creativeMode.png");
	TextureHolder::getInstance().load(Textures::ID::creativeModeSelected, "res/UI/creativeMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::playStartButtonNormal, "res/UI/playStartButton.png");
	TextureHolder::getInstance().load(Textures::ID::playStartButtonSelected, "res/UI/playStartButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::oneStarBadgeNormal, "res/UI/oneStarBadge.png");
	TextureHolder::getInstance().load(Textures::ID::oneStarBadgeBlank, "res/UI/oneStarBadge_b.png");
	TextureHolder::getInstance().load(Textures::ID::threeStarBadgeNormal, "res/UI/threeStarBadge.png");
	TextureHolder::getInstance().load(Textures::ID::threeStarBadgeBlank, "res/UI/threeStarBadge_b.png");

	TextureHolder::getInstance().load(Textures::ID::choosePlayerDeco, "res/Background/choosePlayerDeco.png");
	TextureHolder::getInstance().load(Textures::ID::onePlayerButton, "res/UI/onePlayerButton.png");
	TextureHolder::getInstance().load(Textures::ID::twoPlayerButton, "res/UI/twoPlayerButton.png");
	TextureHolder::getInstance().load(Textures::ID::handSprite, "res/UI/handSprite.png");

	//Enemy
	TextureHolder::getInstance().load(Textures::Ghost, "res/Enemy/Ghost/Ghost.png");
	TextureHolder::getInstance().load(Textures::Chicken, "res/Enemy/Chicken/Chicken.png");
	TextureHolder::getInstance().load(Textures::CockRoach, "res/Enemy/CockRoach/CockRoach.png");
	TextureHolder::getInstance().load(Textures::Snail, "res/Enemy/Snail/Snail.png");
	TextureHolder::getInstance().load(Textures::SnailShell, "res/Enemy/Snail/SnailShell.png");

	//Block
	TextureHolder::getInstance().load(Textures::Floor1, "res/Background/Block/Floor1.png");
	TextureHolder::getInstance().load(Textures::UnderFloor1, "res/Background/Block/UnderFloor1.png");
	TextureHolder::getInstance().load(Textures::StaticBlock1, "res/Background/Block/StaticBlock1.png");
	TextureHolder::getInstance().load(Textures::Breakable1, "res/Background/Breakable/Block1.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation1, "res/Background/Breakable/BreakAnimation1.png");
	TextureHolder::getInstance().load(Textures::LuckyBlock1, "res/Background/LuckyBlock/Unbox1.png");
	TextureHolder::getInstance().load(Textures::StaticLuckyBlock1, "res/Background/LuckyBlock/Empty1.png");

	TextureHolder::getInstance().load(Textures::Floor2, "res/Background/Block/Floor2.png");
	TextureHolder::getInstance().load(Textures::UnderFloor2, "res/Background/Block/UnderFloor2.png");
	TextureHolder::getInstance().load(Textures::StaticBlock2, "res/Background/Block/StaticBlock2.png");
	TextureHolder::getInstance().load(Textures::Breakable2, "res/Background/Breakable/Block2.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation2, "res/Background/Breakable/BreakAnimation2.png");
	TextureHolder::getInstance().load(Textures::LuckyBlock2, "res/Background/LuckyBlock/Unbox2.png");
	TextureHolder::getInstance().load(Textures::StaticLuckyBlock2, "res/Background/LuckyBlock/Empty2.png");

	TextureHolder::getInstance().load(Textures::JumpyBlock, "res/Background/JumpyBlock/Block.png");
	TextureHolder::getInstance().load(Textures::SlideBlock, "res/Background/SlideBlock/Block.png");

	//Item
	TextureHolder::getInstance().load(Textures::Coin, "res/Item/Coin.png");
	TextureHolder::getInstance().load(Textures::CoinAnimation, "res/Item/CoinAnimation.png");
	TextureHolder::getInstance().load(Textures::Heart, "res/Item/Heart.png");
	TextureHolder::getInstance().load(Textures::HeartAnimation, "res/Item/StarAnimation.png");
	TextureHolder::getInstance().load(Textures::FireBall, "res/Item/Fireball.png");
	TextureHolder::getInstance().load(Textures::HeartIcon, "res/Hub/HeartIcon.png");

	//Hub
	TextureHolder::getInstance().load(Textures::CoinIcon, "res/Hub/CoinIcon.png");
	TextureHolder::getInstance().load(Textures::ClockIcon, "res/Hub/ClockIcon.png");
	TextureHolder::getInstance().load(Textures::WhiteText, "res/Text/WhiteText.png");
	TextureHolder::getInstance().load(Textures::BlackText, "res/Text/BlackText.png");
}

void Game::loadFonts() {
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
	nGameStateStack.registerState<LoadingState>(States::ID::Loading);	
	nGameStateStack.registerState<ChooseCharState>(States::ID::ChooseCharacter);
}


