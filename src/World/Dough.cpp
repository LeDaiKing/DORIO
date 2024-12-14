#include "Dough.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Category.hpp"
#include "Enemy.hpp"
#include "Item.hpp"
#include "Block.hpp"
#include "Projectile.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
Textures::ID toTextureID(Dough::Type type)
{
	switch (type)
	{
		case Dough::Dough1:
			return Textures::Dough1;
		case Dough::Dough2:
			return Textures::Dough2;

	}
	return Textures::Dough1;
}

Dough::Dough(Type type)
: nType(type)
, nGrowUp(Small)
, Entity(TextureHolder::getInstance().get(toTextureID(type)))
, nBig(TextureHolder::getInstance().get(Textures::BigDough))
, nFireBig(TextureHolder::getInstance().get(Textures::BigDough))
// , nFireBig(TextureHolder::getInstance().get(Textures::FireBigDough))
{
	setUpEntity();
	setAnimationState(State::Idle);
}

// void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
// {
// }

unsigned int Dough::getCategory() const
{
	return Category::PlayerDough;
}

void Dough::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (nTimeDamage > sf::Time::Zero)
	{
		nTimeDamage -= dt;
		if (nTimeDamage <= sf::Time::Zero)
		{
			// if (nCurrentState == State::Hit)
				setAnimationState(State::Idle);
			// nSprite.setAnimationState(State::Idle);
		}
	}

	if (nOnGround) stateJump = 0;
	
	
	Entity::updateCurrent(dt, commands);
	if (nGrowUp == Big)
	{
		if (nBig.getCurrentAnimationID() != State::Sit && nBig.getCurrentAnimationID() != nCurrentState)
		{
			nBig.setAnimationState(nCurrentState);
		}
		if (nDirection != nBig.isFlipped())
		{
			nBig.setFlipped(nDirection);
		}
		nBig.update(dt);
	}
	else if (nGrowUp == FireBig)
	{
		if (nFireBig.getCurrentAnimationID() != State::Sit && nFireBig.getCurrentAnimationID() != nCurrentState)
		{
			nFireBig.setAnimationState(nCurrentState);
		}
		if (nDirection != nFireBig.isFlipped())
		{
			nFireBig.setFlipped(nDirection);
		}
		nFireBig.update(dt);
	}

	if (nOnGround) std::cout << "On Ground" << std::endl;
	else std::cout << "Not On Ground" << std::endl;
}

void Dough::setUpEntity()
{
	switch (nType)
	{
	case Dough1:
	case Dough2:
		nHitBox = sf::Vector2f(25.f, 28.f);
		nBigHitBox = sf::Vector2f(32.f, 62.f);
		nSpeed = sf::Vector2f(1024.f, 128.f);
		nMaxVelocity = sf::Vector2f(200.f, 712.f);
		nJumpVelocity = 400;
		nJumpVelocity2 = 280;
		nHitPoints = 10;
		break;
	
	default:
		break;
	}

	switch (nType)
	{
	case Dough1:
		addAnimationState(State::Idle, 64, 12, sf::seconds(1.f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Walk, 0, 4, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Jump, 32, 14, sf::seconds(1.f), sf::Vector2i(32, 32), false);
		addAnimationState(State::DoubleJump, 32, 14, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
		addAnimationState(State::Hit, 96, 9, sf::seconds(0.3f), sf::Vector2i(32, 32), true);
		break;
	case Dough2:
		addAnimationState(State::Idle, 96, 11, sf::seconds(1.f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Walk, 160, 12, sf::seconds(0.6f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Jump, 128, 1, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
		addAnimationState(State::DoubleJump, 0, 6, sf::seconds(0.1f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Fall, 32, 1, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
		addAnimationState(State::Hit, 64, 7, sf::seconds(0.3f), sf::Vector2i(32, 32), true);
		break;
	
	default:

		break;
	}
	centerOrigin(nBig);
	nBig.addAnimationState(State::Idle, 60, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), true);
	nBig.addAnimationState(State::Walk, 0, 3, sf::seconds(0.5f), sf::Vector2i(32, 60), true);
	nBig.addAnimationState(State::Jump, 120, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), false);
	nBig.addAnimationState(State::DoubleJump, 120, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), false);
	nBig.addAnimationState(State::Sit, 180, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), true);
	nBig.addAnimationState(State::Fall, 240, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), false);	

	centerOrigin(nFireBig);
	nFireBig.addAnimationState(State::Idle, 60, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), true);
	nFireBig.addAnimationState(State::Walk, 0, 3, sf::seconds(0.5f), sf::Vector2i(32, 60), true);
	nFireBig.addAnimationState(State::Jump, 120, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), false);
	nFireBig.addAnimationState(State::DoubleJump, 120, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), false);
	nFireBig.addAnimationState(State::Sit, 180, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), true);
	nFireBig.addAnimationState(State::Fall, 240, 1, sf::seconds(0.5f), sf::Vector2i(32, 60), false);	
}

void Dough::jump()
{
	if (stateJump == 0)
	{
		if (nCurrentState == State::Fall)
		{
			setAnimationState(State::DoubleJump);
			// addVelocity(0.f, -getVelocity().y);
			setVelocity(getVelocity().x, -nJumpVelocity);
			stateJump = 2;
			return;
		}
		Entity::jump();
		stateJump++;
	} 
	else if (stateJump == 1)
	{
		if (nCurrentState != State::Hit)
			setAnimationState(State::DoubleJump);
		// addVelocity(0.f, -getVelocity().y);
		setVelocity(getVelocity().x, -nJumpVelocity);
		stateJump++;
	} 
}

void Dough::attackEnemy(Enemy& enemy)
{
	enemy.getDamage(1);
}

void Dough::getDamage(int damage)
{
	// setAnimationState(State::Hit);
	if (nCurrentState == State::Hit)
		return;
	
	nTimeDamage = sf::seconds(0.35f);
	if (nGrowUp == Big || nGrowUp == FireBig)
	{
		nGrowUp = Small;
	}
	nSprite.setAnimationState(State::Hit);
}

void Dough::handleCollisionEnemies(SceneNode& graph)
{
	if (nCurrentState == State::Hit)
		return;
	
	if (graph.getCategory() & Category::Enemy && !graph.isMarkedForRemoval())
	{
		Enemy& enemy = static_cast<Enemy&>(graph);
		sf::FloatRect bound = getBoundingRect();
		sf::FloatRect enemyBound = enemy.getBoundingRect();
		collision::Side side = checkCollisionSide(bound, enemyBound);
		if (side == collision::Side::Top)
		{
			attackEnemy(enemy);
		}
		else if (side != collision::Side::None)
		{
			enemy.attackPlayer(*this);
		}

		enemy.isTargetInRange(getPosition());
	}

	for (Ptr& child : graph.getChildren())
    {
        handleCollisionEnemies(*child);
    }
}

void Dough::handleCollisionItems(SceneNode& graph)
{
	if (graph.getCategory() & Category::Item)
	{
		Item& item = static_cast<Item&>(graph);
		sf::FloatRect bound = getBoundingRect();
		sf::FloatRect itemBound = item.getBoundingRect();
		collision::Side side = checkCollisionSide(bound, itemBound);
		if (side != collision::Side::None && !item.isMarkedForRemoval())
		{
			item.activate(*this);
		}
	}

	for (Ptr& child : graph.getChildren())
	{
		handleCollisionItems(*child);
	}
}


int Dough::getStateJump() const
{
	return stateJump;
}

void Dough::setStateJump(int state)
{
	stateJump = state;
}

void Dough::sit()
{
	if (nGrowUp == Small) return;
	if (nGrowUp == Big)
	{
		nBig.setAnimationState(State::Sit);
	}
	else if (nGrowUp == FireBig)
	{
		nFireBig.setAnimationState(State::Sit);
	}
	// nBig.setAnimationState(State::Sit);
}

void Dough::standUp()
{
	if (nGrowUp == Small) return;
	if (nGrowUp == Big)
	{
		nBig.setAnimationState(State::Idle);
	}
	else if (nGrowUp == FireBig)
	{
		nFireBig.setAnimationState(State::Idle);
	}
	// nBig.setAnimationState(nCurrentState);
}

sf::FloatRect Dough::getBoundingRect() const
{
	if (nGrowUp == Small) return Entity::getBoundingRect();
	

	sf::FloatRect bound;
	if (nGrowUp == Big)
	{
		bound = getWorldTransform().transformRect(nBig.getGlobalBounds());
		if (nBig.getCurrentAnimationID() != State::Sit)
		{
			bound.top -= (nBigHitBox.y - bound.height);
			bound.left -= (nBigHitBox.x - bound.width) / 2;
			bound.height = nBigHitBox.y;
			bound.width = nBigHitBox.x;
		}
		else
		{
			bound.top -= (nHitBox.y - bound.height);
			bound.left -= (nHitBox.x - bound.width) / 2;
			bound.height = nHitBox.y;
			bound.width = nHitBox.x;
		}
	}
	else if (nGrowUp == FireBig)
	{
		bound = getWorldTransform().transformRect(nFireBig.getGlobalBounds());
		if (nFireBig.getCurrentAnimationID() != State::Sit)
		{
			bound.top -= (nBigHitBox.y - bound.height);
			bound.left -= (nBigHitBox.x - bound.width) / 2;
			bound.height = nBigHitBox.y;
			bound.width = nBigHitBox.x;
		}
		else
		{
			bound.top -= (nHitBox.y - bound.height);
			bound.left -= (nHitBox.x - bound.width) / 2;
			bound.height = nHitBox.y;
			bound.width = nHitBox.x;
		}
	}


	return bound;
	
}

// void Dough::setAnimationState(State type)
// {
// 	if (nGrowUp == Small)
// 	{
// 		Entity::setAnimationState(type);
// 		return;
// 	}

// 	if (nCurrentState == State::Dead || type == nCurrentState)
// 		return;

// 	nCurrentState = type;
// 	if (nSprite.getCurrentAnimationID() == State::Sit) return;
// 	nSprite.setAnimationState(type);
// }

void Dough::growUPBig()
{
	if (nGrowUp == Big || nGrowUp == FireBig) return;
	nGrowUp = Big;
	nBig.setAnimationState(State::Idle);
}

void Dough::growUPFireBig()
{
	if (nGrowUp == FireBig) return;
	nGrowUp = FireBig;
	nFireBig.setAnimationState(State::Idle);
}

void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (nGrowUp == Small)
	{
		Entity::drawCurrent(target, states);
		return;
	}
	sf::FloatRect bounds = getBoundingRect();
	sf::RectangleShape rect(sf::Vector2f(bounds.width, bounds.height));
	rect.setPosition(bounds.left, bounds.top);
	rect.setFillColor(sf::Color(255, 255, 255, 0));
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(1.f);
	target.draw(rect);

	if (nGrowUp == Big)
	target.draw(nBig, states);
	else if (nGrowUp == FireBig)
	target.draw(nFireBig, states);
}


void Dough::fire(CommandQueue& commands){
	if (nGrowUp != FireBig) return;
	Command fire;
	fire.category = Category::ItemScene;
	fire.action = [this] (SceneNode& node, sf::Time dt)
	{
		std::unique_ptr<Projectile> fireBall(new Projectile(Item::FireBall, getPosition() - sf::Vector2f(0.f, 10.f)));
		if (nFireBig.getCurrentAnimationID() == State::Sit) fireBall->move(0.f, 20.f);
		fireBall->setTargetCategory(Category::Enemy);
		fireBall->setSpeed(312.f);
		if (nDirection) fireBall->setTargetDirection(sf::Vector2f(-1.f, 0.f));
		else fireBall->setTargetDirection(sf::Vector2f(1.f, 0.f));
		node.attachChild(std::move(fireBall));
	};
	commands.push(fire);
}

void Dough::addHitPoints(int points)
{
	nHitPoints += points;
}