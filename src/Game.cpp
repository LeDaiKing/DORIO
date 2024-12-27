#include "Game.hpp"
#include <iostream>
#include "StateStack/TitleState.hpp"
#include "StateStack/GameState.hpp"
#include "StateStack/PauseState.hpp"
#include "StateStack/ChooseModeState.hpp"
#include "StateStack/ChoosePlayerState.hpp"
#include "StateStack/LoadingState.hpp"
#include "StateStack/ChooseCharState.hpp"
#include "StateStack/ChooseSlotState.hpp"
#include "StateStack/InstructionState.hpp"
#include "StateStack/WinningState.hpp"
#include "StateStack/ChooseModeState.hpp"
#include "StateStack/CreativeState.hpp"
#include "StateStack/MusicState.hpp"
#include "StateStack/TransitionState.hpp"
#include "StateStack/LosingState.hpp"

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
	TextureHolder::getInstance().load(Textures::ID::LeaderboardScreen, "res/Background/scoreboard.png");
	TextureHolder::getInstance().load(Textures::ID::LosingScreen, "res/Background/losingScreen.png");

	//Button
	TextureHolder::getInstance().load(Textures::ID::SlotButtonNormal, "res/UI/slotButton.png");
	TextureHolder::getInstance().load(Textures::ID::SlotButtonSelected, "res/UI/slotButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::SlotButtonPressed, "res/UI/slotButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::BackButtonNormal, "res/UI/BackButton.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonSelected, "res/UI/BackButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::BackButtonPressed, "res/UI/BackButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::StartButtonNormal, "res/UI/startButton.png");
	TextureHolder::getInstance().load(Textures::ID::StartButtonSelected, "res/UI/startButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::StartButtonPressed, "res/UI/startButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::DeleteButtonNormal, "res/UI/deleteButton.png");
	TextureHolder::getInstance().load(Textures::ID::DeleteButtonSelected, "res/UI/deleteButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::DeleteButtonPressed, "res/UI/deleteButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::ResetButtonNormal, "res/UI/resetButton.png");
	TextureHolder::getInstance().load(Textures::ID::ResetButtonSelected, "res/UI/resetButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::ResetButtonPressed, "res/UI/resetButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::UpButtonNormal, "res/UI/upButton.png");
	TextureHolder::getInstance().load(Textures::ID::UpButtonSelected, "res/UI/upButton.png");
	TextureHolder::getInstance().load(Textures::ID::UpButtonPressed, "res/UI/upButton.png");

	TextureHolder::getInstance().load(Textures::ID::LeftButtonNormal, "res/UI/leftButton.png");
	TextureHolder::getInstance().load(Textures::ID::LeftButtonSelected, "res/UI/leftButton.png");
	TextureHolder::getInstance().load(Textures::ID::LeftButtonPressed, "res/UI/leftButton.png");

	TextureHolder::getInstance().load(Textures::ID::RightButtonNormal, "res/UI/rightButton.png");
	TextureHolder::getInstance().load(Textures::ID::RightButtonSelected, "res/UI/rightButton.png");
	TextureHolder::getInstance().load(Textures::ID::RightButtonPressed, "res/UI/rightButton.png");

	TextureHolder::getInstance().load(Textures::ID::DownButtonNormal, "res/UI/downButton.png");
	TextureHolder::getInstance().load(Textures::ID::DownButtonSelected, "res/UI/downButton.png");
	TextureHolder::getInstance().load(Textures::ID::DownButtonPressed, "res/UI/downButton.png");

	TextureHolder::getInstance().load(Textures::ID::AttackButtonNormal, "res/UI/attackButton.png");
	TextureHolder::getInstance().load(Textures::ID::AttackButtonSelected, "res/UI/attackButton.png");
	TextureHolder::getInstance().load(Textures::ID::AttackButtonPressed, "res/UI/attackButton.png");

	TextureHolder::getInstance().load(Textures::ID::DoubleUpButtonNormal, "res/UI/doubleUpButton.png");
	TextureHolder::getInstance().load(Textures::ID::DoubleUpButtonSelected, "res/UI/doubleUpButton.png");
	TextureHolder::getInstance().load(Textures::ID::DoubleUpButtonPressed, "res/UI/doubleUpButton.png");

	TextureHolder::getInstance().load(Textures::ID::RectButtonNormal, "res/UI/rectButton.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonSelected, "res/UI/rectButton.png");
	TextureHolder::getInstance().load(Textures::ID::RectButtonPressed, "res/UI/rectButton.png");

	TextureHolder::getInstance().load(Textures::ID::HomeButtonNormal, "res/UI/chooseCharButton.png");
	TextureHolder::getInstance().load(Textures::ID::HomeButtonSelected, "res/UI/chooseCharButton.png");
	TextureHolder::getInstance().load(Textures::ID::HomeButtonPressed, "res/UI/chooseCharButton.png");

	TextureHolder::getInstance().load(Textures::ID::LeaderboardButtonNormal, "res/UI/leaderboardButton.png");
	TextureHolder::getInstance().load(Textures::ID::LeaderboardButtonSelected, "res/UI/leaderboardButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::LeaderboardButtonPressed, "res/UI/leaderboardButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::CharSlot1Normal, "res/UI/charSlot.png");
	TextureHolder::getInstance().load(Textures::ID::CharSlot1Pressed, "res/UI/charSlot_s.png");
	TextureHolder::getInstance().load(Textures::ID::CharSlot1Selected, "res/UI/charSlot_s.png");

	TextureHolder::getInstance().load(Textures::ID::CharSlot2Normal, "res/UI/char2Slot.png");
	TextureHolder::getInstance().load(Textures::ID::CharSlot2Pressed, "res/UI/char2Slot_s.png");
	TextureHolder::getInstance().load(Textures::ID::CharSlot2Selected, "res/UI/char2Slot_s.png");

	TextureHolder::getInstance().load(Textures::ID::PreviousButtonNormal, "res/UI/previousButton.png");
	TextureHolder::getInstance().load(Textures::ID::PreviousButtonSelected, "res/UI/previousButton.png");
	TextureHolder::getInstance().load(Textures::ID::PreviousButtonPressed, "res/UI/previousButton.png");

	TextureHolder::getInstance().load(Textures::ID::NextButtonNormal, "res/UI/nextButton.png");
	TextureHolder::getInstance().load(Textures::ID::NextButtonSelected, "res/UI/nextButton.png");
	TextureHolder::getInstance().load(Textures::ID::NextButtonPressed, "res/UI/nextButton.png");

    TextureHolder::getInstance().load(Textures::ID::Char1Sprite, "res/UI/char1Sprite.png");
	TextureHolder::getInstance().load(Textures::ID::Char2Sprite, "res/UI/char2Sprite.png");

	TextureHolder::getInstance().load(Textures::ID::SaveButtonNormal, "res/UI/SaveButton.png");
	TextureHolder::getInstance().load(Textures::ID::SaveButtonSelected, "res/UI/SaveButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::SaveButtonPressed, "res/UI/SaveButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::InstructionButtonNormal, "res/UI/instructionButton1.png");
	TextureHolder::getInstance().load(Textures::ID::InstructionButtonSelected, "res/UI/instructionButton1_s.png");
	TextureHolder::getInstance().load(Textures::ID::InstructionButtonPressed, "res/UI/instructionButton1_p.png");

	TextureHolder::getInstance().load(Textures::ID::VolumeIncreaseButtonNormal, "res/UI/IncreaseButton.png");
	TextureHolder::getInstance().load(Textures::ID::VolumeIncreaseButtonSelected, "res/UI/IncreaseButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::VolumeIncreaseButtonPressed, "res/UI/IncreaseButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::VolumeDecreaseButtonNormal, "res/UI/DecreaseButton.png");
	TextureHolder::getInstance().load(Textures::ID::VolumeDecreaseButtonSelected, "res/UI/DecreaseButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::VolumeDecreaseButtonPressed, "res/UI/DecreaseButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::ChooseCharButtonNormal, "res/UI/chooseCharButton.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseCharButtonSelected, "res/UI/chooseCharButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseCharButtonPressed, "res/UI/chooseCharButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::CharIntro, "res/UI/charIntro.png");
	TextureHolder::getInstance().load(Textures::ID::CharIntro2, "res/UI/charIntro2.png");
	TextureHolder::getInstance().load(Textures::ID::VolumeBlock, "res/UI/volumeBlock.png");

	TextureHolder::getInstance().load(Textures::ID::ChooseModeButtonNormal, "res/UI/chooseModeButton.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseModeButtonSelected, "res/UI/chooseModeButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::ChooseModeButtonPressed, "res/UI/chooseModeButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::PaintButtonNormal, "res/UI/paintButton.png");
	TextureHolder::getInstance().load(Textures::ID::PaintButtonSelected, "res/UI/paintButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::PaintButtonPressed, "res/UI/paintButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::HandButtonNormal, "res/UI/handButton.png");
	TextureHolder::getInstance().load(Textures::ID::HandButtonSelected, "res/UI/handButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::HandButtonPressed, "res/UI/handButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::EraseButtonNormal, "res/UI/eraseButton.png");
	TextureHolder::getInstance().load(Textures::ID::EraseButtonSelected, "res/UI/eraseButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::EraseButtonPressed, "res/UI/eraseButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::RedoButtonNormal, "res/UI/redoButton.png");
	TextureHolder::getInstance().load(Textures::ID::RedoButtonSelected, "res/UI/redoButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::RedoButtonPressed, "res/UI/redoButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::UndoButtonNormal, "res/UI/undoButton.png");
	TextureHolder::getInstance().load(Textures::ID::UndoButtonSelected, "res/UI/undoButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::UndoButtonPressed, "res/UI/undoButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::SettingButtonNormal, "res/UI/settingButton.png");
	TextureHolder::getInstance().load(Textures::ID::SettingButtonSelected, "res/UI/settingButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::SettingButtonPressed, "res/UI/settingButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::MusicButtonNormal, "res/UI/musicButton.png");
	TextureHolder::getInstance().load(Textures::ID::MusicButtonSelected, "res/UI/musicButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::MusicButtonPressed, "res/UI/musicButton_p.png");

	TextureHolder::getInstance().load(Textures::ID::PurpleRectangle, "res/UI/purpleRectangle.png");

	TextureHolder::getInstance().load(Textures::ID::KitchenModeNormal, "res/UI/kitchenMode.png");
	TextureHolder::getInstance().load(Textures::ID::KitchenModeSelected, "res/UI/kitchenMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::KitchenModePressed, "res/UI/kitchenMode_s.png");

	TextureHolder::getInstance().load(Textures::ID::HallwayModeNormal, "res/UI/hallwayMode.png");
	TextureHolder::getInstance().load(Textures::ID::HallwayModeSelected, "res/UI/hallwayMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::HallwayModePressed, "res/UI/hallwayMode_s.png");

	TextureHolder::getInstance().load(Textures::ID::GardenModeNormal, "res/UI/gardenMode.png");
	TextureHolder::getInstance().load(Textures::ID::GardenModeSelected, "res/UI/gardenMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::GardenModePressed, "res/UI/gardenMode_s.png");

	TextureHolder::getInstance().load(Textures::ID::CreativeModeNormal, "res/UI/creativeMode.png");
	TextureHolder::getInstance().load(Textures::ID::CreativeModeSelected, "res/UI/creativeMode_s.png");
	TextureHolder::getInstance().load(Textures::ID::CreativeModePressed, "res/UI/creativeMode_s.png");

	TextureHolder::getInstance().load(Textures::ID::PlayStartButtonNormal, "res/UI/playStartButton.png");
	TextureHolder::getInstance().load(Textures::ID::PlayStartButtonSelected, "res/UI/playStartButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::PlayStartButtonPressed, "res/UI/playStartButton_s.png");


	TextureHolder::getInstance().load(Textures::ID::OneStarBadgeNormal, "res/UI/oneStarBadge.png");
	TextureHolder::getInstance().load(Textures::ID::OneStarBadgeBlank, "res/UI/oneStarBadge_b.png");
	TextureHolder::getInstance().load(Textures::ID::ThreeStarBadgeNormal, "res/UI/threeStarBadge.png");
	TextureHolder::getInstance().load(Textures::ID::ThreeStarBadgeBlank, "res/UI/threeStarBadge_b.png");

	TextureHolder::getInstance().load(Textures::ID::ChoosePlayerDeco, "res/Background/choosePlayerDeco.png");

	TextureHolder::getInstance().load(Textures::ID::OnePlayerButtonNormal, "res/UI/onePlayerButton.png");
	TextureHolder::getInstance().load(Textures::ID::OnePlayerButtonSelected, "res/UI/onePlayerButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::OnePlayerButtonPressed, "res/UI/onePlayerButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::TwoPlayerButtonNormal, "res/UI/twoPlayerButton.png");
	TextureHolder::getInstance().load(Textures::ID::TwoPlayerButtonSelected, "res/UI/twoPlayerButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::TwoPlayerButtonPressed, "res/UI/twoPlayerButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::RetryButtonNormal, "res/UI/retryButton.png");
	TextureHolder::getInstance().load(Textures::ID::RetryButtonSelected, "res/UI/retryButton_s.png");
	TextureHolder::getInstance().load(Textures::ID::RetryButtonPressed, "res/UI/retryButton_s.png");

	TextureHolder::getInstance().load(Textures::ID::HandSprite, "res/UI/handSprite.png");
	TextureHolder::getInstance().load(Textures::ID::TextBox, "res/UI/textBox.png");
	
	TextureHolder::getInstance().load(Textures::Dough1, "res/Dough/dough.png");
	TextureHolder::getInstance().load(Textures::Dough2, "res/Dough/dough1.png");
	TextureHolder::getInstance().load(Textures::BigDough2, "res/Dough/big_dough.png");
	TextureHolder::getInstance().load(Textures::FireBigDough2, "res/Dough/fire_dough.png");

	//Enemy
	TextureHolder::getInstance().load(Textures::Ghost, "res/Enemy/Ghost/Ghost.png");
	TextureHolder::getInstance().load(Textures::Chicken, "res/Enemy/Chicken/Chicken.png");
	TextureHolder::getInstance().load(Textures::CockRoach, "res/Enemy/CockRoach/CockRoach.png");
	TextureHolder::getInstance().load(Textures::Snail, "res/Enemy/Snail/Snail.png");
	TextureHolder::getInstance().load(Textures::SnailShell, "res/Enemy/Snail/SnailShell.png");
	TextureHolder::getInstance().load(Textures::HugeEnemy, "res/Enemy/HugeEnemy/witch.png");
	TextureHolder::getInstance().load(Textures::HugeEnemyWarning, "res/Enemy/HugeEnemy/eyeball.png");

	//Block
	TextureHolder::getInstance().load(Textures::Floor1, "res/Background/Block/Floor1.png");
	TextureHolder::getInstance().load(Textures::UnderFloor1, "res/Background/Block/UnderFloor1.png");
	TextureHolder::getInstance().load(Textures::StaticBlock1, "res/Background/Block/StaticBlock1.png");
	TextureHolder::getInstance().load(Textures::Breakable1, "res/Background/Breakable/Block1.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation1, "res/Background/Breakable/BreakAnimation1.png");
	TextureHolder::getInstance().load(Textures::LuckyBlock1, "res/Background/LuckyBlock/Unbox1.png");
	TextureHolder::getInstance().load(Textures::StaticLuckyBlock1, "res/Background/LuckyBlock/Empty1.png");
	TextureHolder::getInstance().load(Textures::Gold, "res/Background/Block/Gold.png");

	TextureHolder::getInstance().load(Textures::Floor2, "res/Background/Block/Floor2.png");
	TextureHolder::getInstance().load(Textures::UnderFloor2, "res/Background/Block/UnderFloor2.png");
	TextureHolder::getInstance().load(Textures::StaticBlock2, "res/Background/Block/StaticBlock2.png");
	TextureHolder::getInstance().load(Textures::Breakable2, "res/Background/Breakable/Block2.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation2, "res/Background/Breakable/BreakAnimation2.png");
	TextureHolder::getInstance().load(Textures::LuckyBlock2, "res/Background/LuckyBlock/Unbox2.png");
	TextureHolder::getInstance().load(Textures::StaticLuckyBlock2, "res/Background/LuckyBlock/Empty2.png");

	TextureHolder::getInstance().load(Textures::JumpyBlock, "res/Background/JumpyBlock/Block.png");
	TextureHolder::getInstance().load(Textures::SlideBlock, "res/Background/SlideBlock/Block.png");

	//Pipe
	TextureHolder::getInstance().load(Textures::SewerPipe, "res/Background/Pipes/SewerPipe.png");
	TextureHolder::getInstance().load(Textures::Pipe, "res/Background/Pipes/Pipe.png");

	//Item
	TextureHolder::getInstance().load(Textures::Coin, "res/Item/Coin.png");
	TextureHolder::getInstance().load(Textures::CoinAnimation, "res/Item/CoinAnimation.png");
	TextureHolder::getInstance().load(Textures::Heart, "res/Item/life.png");
	TextureHolder::getInstance().load(Textures::HeartAnimation, "res/Item/lifeAni.png");
	TextureHolder::getInstance().load(Textures::FireBall, "res/Item/Fireball.png");
	TextureHolder::getInstance().load(Textures::BigItem, "res/Item/big_potion.png");
	TextureHolder::getInstance().load(Textures::BigItemAnimation, "res/Item/BigPotionAni.png");
	TextureHolder::getInstance().load(Textures::FireBigItem, "res/Item/fire_potion.png");
	TextureHolder::getInstance().load(Textures::FireBigItemAnimation, "res/Item/FirePotionAni.png");

	//Hub
	TextureHolder::getInstance().load(Textures::HeartIcon, "res/Hub/HeartIcon.png");
	TextureHolder::getInstance().load(Textures::CoinIcon, "res/Hub/CoinIcon.png");
	TextureHolder::getInstance().load(Textures::ClockIcon, "res/Hub/ClockIcon.png");
	TextureHolder::getInstance().load(Textures::WhiteText, "res/Text/WhiteText.png");
	TextureHolder::getInstance().load(Textures::BlackText, "res/Text/BlackText.png");
	TextureHolder::getInstance().load(Textures::ClockHub, "res/Hub/ClockHub.png");
	TextureHolder::getInstance().load(Textures::HeartHub, "res/Hub/HeartHub.png");

	// CheckPoint
	TextureHolder::getInstance().load(Textures::Start, "res/Background/CheckPoint/Start.png");
	TextureHolder::getInstance().load(Textures::Checkpoint, "res/Background/CheckPoint/Checkpoint.png");
	TextureHolder::getInstance().load(Textures::End, "res/Background/CheckPoint/End.png");
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
// 	nStatisticsUpdateTime += elapsedTime;
// 	nStatisticsNumFrames += 1;

// 	if (nStatisticsUpdateTime >= sf::seconds(1.0f))
// 	{
// 		nStatisticsText.setString(
// 			"Frames / Second = " + std::to_string(nStatisticsNumFrames) + "\n" +
// 			"Time / Update = " + std::to_string(nStatisticsUpdateTime.asMicroseconds() / nStatisticsNumFrames) + "us");

// 		std::cout << "Frames / Second = " << nStatisticsNumFrames << std::endl;
// 		std::cout << "Time / Update = " << nStatisticsUpdateTime.asMicroseconds() / nStatisticsNumFrames << "us" << std::endl;		 
// 		nStatisticsUpdateTime -= sf::seconds(1.0f);
// 		nStatisticsNumFrames = 0;
// 	}
}

void Game::registerStates() {
	nGameStateStack.registerState<TitleState>(States::ID::Title);
	nGameStateStack.registerState<GameState>(States::ID::Game);
	nGameStateStack.registerState<PauseState>(States::ID::Pause);
	nGameStateStack.registerState<ChooseModeState>(States::ID::ChooseMode);
	nGameStateStack.registerState<ChoosePlayerState>(States::ID::ChoosePlayer);
	nGameStateStack.registerState<LoadingState>(States::ID::Loading);	
	nGameStateStack.registerState<ChooseCharState>(States::ID::ChooseCharacter);
	nGameStateStack.registerState<ChooseSlotState>(States::ID::ChooseSlot);
	nGameStateStack.registerState<InstructionState>(States::ID::Instruction);
	nGameStateStack.registerState<WinningState>(States::ID::Winning);
	nGameStateStack.registerState<CreativeState>(States::ID::Creative);
	nGameStateStack.registerState<MusicState>(States::ID::Music);
	nGameStateStack.registerState<TransitionState>(States::ID::Transition);
	nGameStateStack.registerState<LosingState>(States::ID::Losing);
}


