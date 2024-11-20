#include "Player.hpp"
#include "Command/CommandQueue.hpp"
#include "World/Dough.hpp"

#include <map>
#include <string>
#include <algorithm>


struct DoughMover
{
	DoughMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	void operator() (Dough& Dough, sf::Time) const
	{
		Dough.accelerate(velocity);
		Dough.setAnimationID(2);
	}

	sf::Vector2f velocity;
};

struct DoughTurner
{
	DoughTurner(bool left)
	: nLeft(left)
	{
	}

	void operator() (Dough& Dough, sf::Time) const
	{
		if (nLeft)
			Dough.turnLeft();
		else
			Dough.turnRight();
	}

	bool nLeft;
};

struct DoughStopper
{
	void operator() (Dough& Dough, sf::Time) const
	{
		Dough.setAnimationID(0);
	}
};

Player::Player()
{
	// Set initial key bindings
	nKeyBinding[sf::Keyboard::Left] = MoveLeft;
	nKeyBinding[sf::Keyboard::Right] = MoveRight;
	nKeyBinding[sf::Keyboard::Up] = MoveUp;
	nKeyBinding[sf::Keyboard::Down] = MoveDown;

	

	// Set initial action bindings
	initializeActions();	

	// Assign all categories to player's Dough
	for(auto& pair : nActionBinding)
		pair.second.category = Category::PlayerDough;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = nKeyBinding.find(event.key.code);
		if (found != nKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(nActionBinding[found->second]);

		if (event.key.code == sf::Keyboard::Left)
			commands.push(nActionBinding[TurnLeft]);

		if (event.key.code == sf::Keyboard::Right)
			commands.push(nActionBinding[TurnRight]);
	}

	if (event.type == sf::Event::KeyReleased)
	{
		// Check if released key appears in key binding, trigger command if so
		auto found = nKeyBinding.find(event.key.code);
		if (found != nKeyBinding.end())
			commands.push(nActionBinding[Idle]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for(auto pair : nKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(nActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = nKeyBinding.begin(); itr != nKeyBinding.end(); )
	{
		if (itr->second == action)
			nKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	nKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for(auto pair : nKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float playerSpeed = 150.f;

	nActionBinding[Idle].action		= derivedAction<Dough>(DoughStopper());
	nActionBinding[MoveLeft].action	 = derivedAction<Dough>(DoughMover(-playerSpeed, 0.f));
	nActionBinding[MoveRight].action = derivedAction<Dough>(DoughMover(+playerSpeed, 0.f));
	nActionBinding[MoveUp].action    = derivedAction<Dough>(DoughMover(0.f, -playerSpeed));
	nActionBinding[MoveDown].action  = derivedAction<Dough>(DoughMover(0.f, +playerSpeed));
	nActionBinding[TurnLeft].action  = derivedAction<Dough>(DoughTurner(true));
	nActionBinding[TurnRight].action = derivedAction<Dough>(DoughTurner(false));

}



bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
			return true;

		default:
			return false;
	}
}