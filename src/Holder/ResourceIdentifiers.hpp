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
		Sky,
		Dirt,
		Breakable,
		BreakAnimation,
		Coin,
		Stone,
		UnderDirt,
		Gold,
		Chimney,
		Wonder,
		Move,
		Break,
		Loxo
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