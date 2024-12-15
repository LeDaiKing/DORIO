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
        // Add an entry for each new texture
		Dough1,
		Dough2,
		Enemy,
		Ghost,
		Chicken,
		CockRoach,
		Snail,
		SnailShell,
		Sky,
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
		Dirt,
		Breakable,
		BreakAnimation,
		LuckyBlock,
		StaticLuckyBlock,
		JumpyBlock,
		Coin,
		CoinAnimation,
		Heart,
		HeartAnimation,
		BigItem,
		BigItemAnimation,
		FireBigItem,
		FireBigItemAnimation,
		FireBall,
		BigDough,
		FireBigDough,
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