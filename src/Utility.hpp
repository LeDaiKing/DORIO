#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <nlohmann/json.hpp>

#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#define var2Str(var) #var
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
// const float 	

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
bool checkCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
collision::Side checkCollisionSide(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
// bool checkCollisionLC(const sf::FloatRect rect1, const sf::FloatRect rect2);
// bool checkCollisionRC(const sf::FloatRect rect1, const sf::FloatRect rect2);


bool checkInRange(const sf::FloatRect& rect1, const sf::Vector2f& range);

// convert json to vector2
template <typename T>
sf::Vector2<T> toVector2(const nlohmann::json& json);


//file/folder

bool isFileEmpty(const std::string& fileName);
bool isFolderEmpty(const std::string& folderName);
void clearFolder(const std::string& folderName);
void getPreviousFolder(std::string& folderName);

#include "Utility.inl"

