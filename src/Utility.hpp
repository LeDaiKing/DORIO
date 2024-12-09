#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

namespace collision
{
	enum Side
	{
		None,
		Top,
		Bottom,
		Left,
		Right,
	};
}

const float		PI = 3.14159265358979f;

class Animation;

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string		toString(const T& value);

// Convert enumerators to strings
std::string		toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);
void			centerOrigin(Animation& animation);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);

float    		getAngle(sf::Vector2f vector);
// Random number generation
int				randomInt(int exclusiveMax);

// Vector operations
float			length(sf::Vector2f vector);
sf::Vector2f	unitVector(sf::Vector2f vector);


// Check Collision
bool checkCollision(const sf::FloatRect rect1, const sf::FloatRect rect2);
collision::Side checkCollisionSide(const sf::FloatRect rect1, const sf::FloatRect rect2);
// bool checkCollisionLC(const sf::FloatRect rect1, const sf::FloatRect rect2);
// bool checkCollisionRC(const sf::FloatRect rect1, const sf::FloatRect rect2);
#include "Utility.inl"

