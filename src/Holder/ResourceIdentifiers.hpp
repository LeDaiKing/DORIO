#pragma once
// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
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
		Sky,
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
		FireBall,

	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;