#include "Player.hpp"
#include "Command/CommandQueue.hpp"
#include "World/Dough.hpp"
#include <iostream>
#include <map>
#include <string>
#include <algorithm>


struct DoughMover
{
	DoughMover(bool nDirection)
	: nDirection(nDirection)
	{
	}

	void operator() (Dough& Dough, sf::Time) const
	{
		Dough.walk(nDirection);
	}
	bool nDirection;
};


struct DougJump
{
	DougJump()
	{
	}

	void operator() (Dough& Dough, sf::Time) const
	{
		Dough.jump();
	}

};




Player::Player()
{
	// Set initial key bindings
	nKeyBinding[sf::Keyboard::Left] = MoveLeft;
	nKeyBinding[sf::Keyboard::Right] = MoveRight;
	nKeyBinding[sf::Keyboard::Up] = MoveUp;
	nKeyBinding[sf::Keyboard::A] = MoveLeft1;
	nKeyBinding[sf::Keyboard::D] = MoveRight1;
	nKeyBinding[sf::Keyboard::W] = MoveUp1;
	// nKeyBinding[sf::Keyboard::Down] = Sit;

	

	// Set initial action bindings
	initializeActions();	

	// Assign all categories to player's Dough
	nActionBinding[MoveLeft].category = Category::PlayerDough1;
	nActionBinding[MoveRight].category = Category::PlayerDough1;
	nActionBinding[MoveUp].category = Category::PlayerDough1;
	nActionBinding[Fire].category = Category::PlayerDough1;
	nActionBinding[Sit].category = Category::PlayerDough1;
	nActionBinding[StandUp].category = Category::PlayerDough1;

	nActionBinding[MoveLeft1].category = Category::PlayerDough2;
	nActionBinding[MoveRight1].category = Category::PlayerDough2;
	nActionBinding[MoveUp1].category = Category::PlayerDough2;
	nActionBinding[Fire1].category = Category::PlayerDough2;
	nActionBinding[Sit1].category = Category::PlayerDough2;
	nActionBinding[StandUp1].category = Category::PlayerDough2;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = nKeyBinding.find(event.key.code);
		if (found != nKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(nActionBinding[found->second]);

		if (event.key.code == sf::Keyboard::Down)
		{
			commands.push(nActionBinding[Sit]);
		}

		if (event.key.code == sf::Keyboard::Space)
		{
			commands.push(nActionBinding[Fire]);
		}

		if (event.key.code == sf::Keyboard::S)
		{
			commands.push(nActionBinding[Sit1]);
		}

		if (event.key.code == sf::Keyboard::F)
		{
			commands.push(nActionBinding[Fire1]);
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Down)
		{
			commands.push(nActionBinding[StandUp]);
		}
		if (event.key.code == sf::Keyboard::S)
		{
			commands.push(nActionBinding[StandUp1]);
		}
	}

}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for(auto pair : nKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(nActionBinding[pair.second]);
		}
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
	const float playerSpeed = 512.f;

	nActionBinding[MoveLeft].action	 = derivedAction<Dough>(DoughMover(true));
	nActionBinding[MoveRight].action = derivedAction<Dough>(DoughMover(false));
	nActionBinding[MoveUp].action    = derivedAction<Dough>(DougJump());
	nActionBinding[Sit].action       = derivedAction<Dough>([](Dough& a, sf::Time){ a.sit(); });
	nActionBinding[StandUp].action   = derivedAction<Dough>([](Dough& a, sf::Time){ a.standUp(); });
	nActionBinding[Fire].action      = derivedAction<Dough>([](Dough& a, sf::Time){ a.preFire(); });

	nActionBinding[MoveLeft1].action	 = derivedAction<Dough>(DoughMover(true));
	nActionBinding[MoveRight1].action = derivedAction<Dough>(DoughMover(false));
	nActionBinding[MoveUp1].action    = derivedAction<Dough>(DougJump());
	nActionBinding[Sit1].action       = derivedAction<Dough>([](Dough& a, sf::Time){ a.sit(); });
	nActionBinding[StandUp1].action   = derivedAction<Dough>([](Dough& a, sf::Time){ a.standUp(); });
	nActionBinding[Fire1].action      = derivedAction<Dough>([](Dough& a, sf::Time){ a.preFire(); });
}



bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveLeft1:
		case MoveRight1:
		// case MoveUp:
			return true;

		default:
			return false;
	}
}