#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

namespace Music{
	enum ID
	{
		BackgroundTheme, 
		ClickedButton,
		map1,
		map2,
		map3,
		themesong,	
	};
}

namespace SoundEffect{
	enum ID
	{
		ClickedButton,
		levelcomplete,
		achievement,
		coin,
		gameover,
		hurt,
		item,
		jump,
		land,
		ui,
		powerup,
		shoot,
	};
}

namespace Textures
{
	enum ID
	{
        // Background
		BackGround1,
		BackGround2,
		LeaderboardScreen,
		TitleScreen,
		ChooseModeScreen,
		LosingScreen,
		
		// Button
		RectButtonNormal,
		RectButtonSelected,
		RectButtonPressed,
		SquareButtonNormal,
		SquareButtonSelected,
		SquareButtonPressed,
		BackButtonNormal,
		BackButtonSelected,
		BackButtonPressed,
		ChooseCharScreen,
		ChoosePlayerScreen,
		CharSlot1Normal,
		CharSlot1Selected,
		CharSlot1Pressed,
		CharSlot2Normal,
		CharSlot2Selected,
		CharSlot2Pressed,
		PreviousButtonNormal,
		PreviousButtonSelected,
		PreviousButtonPressed,
		NextButtonNormal, 
		NextButtonSelected,
		NextButtonPressed,
		Char1Sprite,
		Char2Sprite,
		SaveButtonNormal,
		SaveButtonSelected,
		SaveButtonPressed,
		InstructionButtonNormal,
		InstructionButtonSelected,
		InstructionButtonPressed,
		ChooseCharButtonNormal,
		ChooseCharButtonSelected,
		ChooseCharButtonPressed,
		ChooseModeButtonNormal,
		ChooseModeButtonSelected,
		ChooseModeButtonPressed,
		CharIntro,
		CharIntro2,
		KitchenModeNormal,
		KitchenModeSelected,
		KitchenModePressed,
		HallwayModeNormal,
		HallwayModeLocked,
		HallwayModeSelected,
		HallwayModePressed,
		GardenModeNormal,
		GardenModeSelected,
		GardenModeLocked,
		GardenModePressed,
		CreativeModeNormal,
		CreativeModeSelected,
		CreativeModePressed,
		PlayStartButtonNormal,
		PlayStartButtonSelected,
		PlayStartButtonPressed,
		OneStarBadgeNormal,
		OneStarBadgeBlank,
		ThreeStarBadgeNormal,
		ThreeStarBadgeBlank,
		ChoosePlayerDeco,
		OnePlayerButtonNormal,
		OnePlayerButtonSelected,
		OnePlayerButtonPressed,
		TwoPlayerButtonNormal,
		TwoPlayerButtonSelected,
		TwoPlayerButtonPressed,
		HandSprite,
		SlotButtonNormal,
		SlotButtonSelected,
		SlotButtonPressed,
		StartButtonNormal,
		StartButtonNormal1, 
		StartButtonSelected,
		StartButtonPressed,
		DeleteButtonNormal,
		DeleteButtonSelected,
		DeleteButtonPressed,
		ResetButtonNormal,
		ResetButtonSelected,
		ResetButtonPressed,
		UpButtonNormal,
		UpButtonSelected,
		UpButtonPressed,
		LeftButtonNormal,
		LeftButtonSelected,
		LeftButtonPressed,
		RightButtonNormal,
		RightButtonSelected,
		RightButtonPressed,
		DownButtonNormal,
		DownButtonSelected,
		DownButtonPressed,
		AttackButtonNormal,
		AttackButtonSelected,
		AttackButtonPressed,
		DoubleUpButtonNormal,
		DoubleUpButtonSelected,
		DoubleUpButtonPressed,
		UpButtonNormal1,
		UpButtonSelected1,
		UpButtonPressed1,
		LeftButtonNormal1,
		LeftButtonSelected1,
		LeftButtonPressed1,
		RightButtonNormal1,
		RightButtonSelected1,
		RightButtonPressed1,
		DownButtonNormal1,
		DownButtonSelected1,
		DownButtonPressed1,
		AttackButtonNormal1,
		AttackButtonSelected1,
		AttackButtonPressed1,
		DoubleUpButtonNormal1,
		DoubleUpButtonSelected1,
		DoubleUpButtonPressed1,
		HomeButtonNormal,
		HomeButtonSelected,
		HomeButtonPressed,
		LeaderboardButtonNormal,
		LeaderboardButtonSelected,
		LeaderboardButtonPressed,
		PaintButtonNormal,
		PaintButtonSelected,
		PaintButtonPressed,
		HandButtonNormal,
		HandButtonSelected,
		HandButtonPressed,
		EraseButtonNormal,
		EraseButtonSelected,
		EraseButtonPressed,
		RedoButtonNormal,
		RedoButtonSelected,
		RedoButtonPressed,
		UndoButtonNormal,
		UndoButtonSelected,
		UndoButtonPressed,
		SettingButtonNormal,
		SettingButtonSelected,
		SettingButtonPressed,
		MusicButtonNormal,
		MusicButtonSelected,
		MusicButtonPressed,
		VolumeIncreaseButtonNormal,
		VolumeIncreaseButtonSelected,
		VolumeIncreaseButtonPressed,
		VolumeDecreaseButtonNormal,
		VolumeDecreaseButtonSelected,
		VolumeDecreaseButtonPressed,
		Player1ButtonNormal,
		Player1ButtonSelected,
		Player1ButtonPressed,
		Player2ButtonNormal,
		Player2ButtonSelected,
		Player2ButtonPressed,
		TextBox,
		RetryButtonNormal,
		RetryButtonSelected,
		RetryButtonPressed,
		WinningScreen,
		Cup,

		statBox,
		PurpleRectangle,
		VolumeBlock,
		
		// Dough
		Dough1,
		Dough2,
		BigDough1,
		BigDough2,
		FireBigDough1,
		FireBigDough2,

		Ghost,
		Chicken,
		CockRoach,
		Snail,
		SnailShell,
		HugeEnemy,
		HugeEnemyWarning,

		// Block
		Floor1,
		UnderFloor1,
		StaticBlock1,
		Floor2,
		UnderFloor2,
		StaticBlock2,
		Breakable1,
		BreakAnimation1,
		Breakable2,
		BreakAnimation2,
		LuckyBlock1,
		StaticLuckyBlock1,
		LuckyBlock2,
		StaticLuckyBlock2,
		JumpyBlock,
		SlideBlock,
		Gold,

		//Pipe
		SewerPipe,
		Pipe,

		// Item
		Coin,
		CoinAnimation,
		Heart,
		HeartAnimation,
		BigItem,
		BigItemAnimation,
		FireBigItem,
		FireBigItemAnimation,
		FireBall,

		// Hub
		ClockIcon,
		CoinIcon,
		HeartIcon,
		SettingBut,
		WhiteText,
		BlackText,
		ClockHub,
		HeartHub,

		// CheckPoint
		Start,
		Checkpoint,
		End,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Mario,
		Bytebounce,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;