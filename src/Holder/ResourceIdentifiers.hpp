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
	};
}

namespace SoundEffect{
	enum ID
	{
		ClickedButton,
	};
}

namespace Textures
{
	enum ID
	{
        // Background
		BackGround1,
		BackGround2,
		
		// Button
		TitleScreen,
		ChooseModeScreen,
		RectButtonNormal,
		RectButtonSelected,
		RectButtonPressed,
		SquareButtonNormal,
		SquareButtonSelected,
		SquareButtonPressed,
		CreativeButtonNormal, 
		CreativeButtonSelected,
		CreativeButtonPressed,
		BackButtonNormal,
		BackButtonSelected,
		BackButtonPressed,
		ChooseCharScreen,
		
		// Dough
		Dough1,
		Dough2,
		BigDough,
		FireBigDough,

		Ghost,
		Chicken,
		CockRoach,
		Snail,
		SnailShell,

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
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Mario
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;