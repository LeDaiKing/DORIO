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
#include "../ConfigLoader.hpp"
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
, nFireBig(TextureHolder::getInstance().get(Textures::FireBigDough))
, stateJump(0)
, nCoinsCount(0)
, nScore(0)
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
		if (nBig.isFlipped() != nDirection)
			nBig.setFlipped(nDirection);
		
		if (nCurrentState == State::Appear && nBig.isFinished())
		{
			nBig.setAnimationState(State::Idle);
			nCurrentState = State::Idle;
		}

		if (nCurrentState == State::Disappear && nBig.isFinished())
		{
			toSmall();
			return;
		}
		nBig.update(dt);
	}
	else if (nGrowUp == FireBig)
	{
		if (nFireBig.isFlipped() != nDirection)
			nFireBig.setFlipped(nDirection);
		if (nCurrentState == State::Appear && nFireBig.isFinished())
		{
			nFireBig.setAnimationState(State::Idle);
			nCurrentState = State::Idle;
		}

		if (nCurrentState == State::Disappear && nFireBig.isFinished())
		{
			toSmall();
			return;
		}
	
		if (nCurrentState == State::Fire)
		{
			if (nFireBig.getCurrentAnimationID() == State::Sit || (nFireBig.isFinished()))
			{
				fire(commands);
				nCurrentState = State::Idle;
				if (nFireBig.isFinished())
					nFireBig.setAnimationState(State::Idle);
			}
		}
		nFireBig.update(dt);
	}

	// if (nOnGround) std::cout << "On Ground" << std::endl;
	// else std::cout << "Not On Ground" << std::endl;
}

void Dough::setUpEntity()
{
	std::string key = std::string("Dough/") + (nType == Dough1 ? "Dough1" : "Dough2");
	const nlohmann::json& config = ConfigLoader::getInstance().getConfig(key.c_str());
	nHitBox = toVector2<float>(config["HitBox"]);
	nBigHitBox = toVector2<float>(config["BigHitBox"]);
	nSpeed = toVector2<float>(config["Speed"]);
	nMaxVelocity = toVector2<float>(config["MaxVelocity"]);
	nJumpVelocity = config["JumpVelocity"];
	nJumpVelocity2 = config["JumpVelocity2"];
	nHitPoints = config["HitPoints"];

	switch (nType)
	{
	case Dough1:
		addAnimationState(State::Idle, 96, 12, sf::seconds(1.f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Walk, 0, 4, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
		addAnimationState(State::Jump, 32, 5, sf::seconds(0.4f), sf::Vector2i(32, 32), false);
		addAnimationState(State::DoubleJump, 32, 5, sf::seconds(0.4f), sf::Vector2i(32, 32), false);
		addAnimationState(State::Fall, 64, 5, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
		addAnimationState(State::Hit, 128, 9, sf::seconds(0.3f), sf::Vector2i(32, 32), true);
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
	nBig.addAnimationState(State::Idle, 0, 7, sf::seconds(0.7f), sf::Vector2i(32, 64), true);
	nBig.addAnimationState(State::Walk, 64, 8, sf::seconds(0.5f), sf::Vector2i(32, 64), true);
	nBig.addAnimationState(State::Jump, 128, 4, sf::seconds(0.4f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::DoubleJump, 128, 4, sf::seconds(0.4f), sf::Vector2i(32, 64), false);
	// nBig.addAnimationState(State::Sit, 180, 1, sf::seconds(0.5f), sf::Vector2i(32, 64), true);
	nBig.addAnimationState(State::Fall, 192, 5, sf::seconds(0.5f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::Appear, 256, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::Disappear, 256, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);	

	centerOrigin(nFireBig);
	nFireBig.addAnimationState(State::Idle, 0, 7, sf::seconds(0.7f), sf::Vector2i(32, 64), true);
	nFireBig.addAnimationState(State::Walk, 64, 6, sf::seconds(0.5f), sf::Vector2i(32, 64), true);
	nFireBig.addAnimationState(State::Jump, 128, 5, sf::seconds(0.3f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::DoubleJump, 128, 5, sf::seconds(0.3f), sf::Vector2i(32, 64), false);
	// nFireBig.addAnimationState(State::Sit, 180, 1, sf::seconds(0.5f), sf::Vector2i(32, 64), true);
	nFireBig.addAnimationState(State::Fall, 192, 1, sf::seconds(0.5f), sf::Vector2i(32, 64), false);	
	nFireBig.addAnimationState(State::Fire, 256, 6, sf::seconds(0.4f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::Appear, 320, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::Disappear, 320, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);
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
	if (nGrowUp != Small)
	{
		// nGrowUp = Small;
		nCurrentState = State::Disappear;
		if (nGrowUp == Big)
		{
			nBig.setAnimationState(State::Disappear);
		}
		else if (nGrowUp == FireBig)
		{
			nFireBig.setAnimationState(State::Disappear);
		}
	}
	// nSprite.setAnimationState(State::Hit);
	nHitPoints -= damage;
	setAnimationState(State::Hit);
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
		nBig.setAnimationState(nCurrentState);
	}
	else if (nGrowUp == FireBig)
	{
		nFireBig.setAnimationState(nCurrentState);
	}
	// nBig.setAnimationState(nCurrentState);
}

sf::FloatRect Dough::getBoundingRect() const
{
	if (nGrowUp == Small) 
	{
		sf::FloatRect bound = getWorldTransform().transformRect(nSprite.getGlobalBounds());
		bound.top -= (nHitBox.y - bound.height);
		bound.left -= (nHitBox.x - bound.width) / 2;
		bound.height = nHitBox.y;
		bound.width = nHitBox.x;
		return bound;
	}

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

void Dough::setAnimationState(State type)
{
	if (nGrowUp == Small)
	{
		Entity::setAnimationState(type);
		return;
	}

	if (nCurrentState == State::Dead || type == nCurrentState
	|| nCurrentState ==State::Fire || nCurrentState == State::Appear 
	|| nCurrentState == State::Disappear)
		return;

	if (nCurrentState != State::Fire)
		nCurrentState = type;
	if (nGrowUp == Big)
	{
		if (nBig.getCurrentAnimationID() != State::Sit)
		{
			nBig.setAnimationState(type);
		}
		
	}
	else if (nGrowUp == FireBig)
	{
		if (nFireBig.getCurrentAnimationID() != State::Sit)
		{
			nFireBig.setAnimationState(type);
		}
	}
}

void Dough::growUPBig()
{
	addHitPoints(1);
	if (nGrowUp == Big || nGrowUp == FireBig) return;
	nGrowUp = Big;
	nCurrentState = State::Idle;
	nBig.setAnimationState(State::Idle);
}

void Dough::growUPFireBig()
{
	addHitPoints(1);
	if (nGrowUp == FireBig) return;
	nGrowUp = FireBig;
	nCurrentState = State::Appear;
	nFireBig.setAnimationState(State::Appear);
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


void Dough::fire(CommandQueue& commands)
{
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

void Dough::addCoins(int coins)
{
	nCoinsCount += coins;
}
int Dough::getCoinsCount() const
{
	return nCoinsCount;
}

int Dough::getHitPoints() const
{
	return nHitPoints;
}

void Dough::preFire()
{
	if (nGrowUp != FireBig) return;
	if (nFireBig.getCurrentAnimationID() == State::Fire) return;
	setAnimationState(State::Fire);
}

void Dough::toSmall()
{
	if (nGrowUp == Small) return;
	nGrowUp = Small;
	// nCurrentState = State::Idle;
	setAnimationState(State::Idle);
	nBig.setAnimationState(State::Idle);
	nFireBig.setAnimationState(State::Idle);
}

void Dough::setCheckPoint(sf::Vector2f checkPoint)
{
	nCheckPoint = checkPoint;
}

void Dough::resetCheckPoint()
{
	setPosition(nCheckPoint);
}

void Dough::save(std::ofstream& file)
{
	/*
	    type
		growUp
		nCoinsCount
		nScore
		nCheckPoint
	*/
	int type = nType;
	file.write(reinterpret_cast<char*>(&type), sizeof(type));
	file.write(reinterpret_cast<char*>(&nGrowUp), sizeof(nGrowUp));
	file.write(reinterpret_cast<char*>(&nCoinsCount), sizeof(nCoinsCount));
	file.write(reinterpret_cast<char*>(&nScore), sizeof(nScore));
	file.write(reinterpret_cast<char*>(&nCheckPoint), sizeof(nCheckPoint));
	sf::Vector2f pos = getPosition();
	file.write(reinterpret_cast<char*>(&pos), sizeof(pos));
}

void Dough::load(std::ifstream& file)
{
	/*
	    type
		growUp
		nCoinsCount
		nScore
		nCheckPoint
	*/
	file.read(reinterpret_cast<char*>(&nGrowUp), sizeof(nGrowUp));
	file.read(reinterpret_cast<char*>(&nCoinsCount), sizeof(nCoinsCount));
	file.read(reinterpret_cast<char*>(&nScore), sizeof(nScore));
	file.read(reinterpret_cast<char*>(&nCheckPoint), sizeof(nCheckPoint));

	sf::Vector2f pos;
	file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
	setPosition(pos);

	if (nGrowUp == Big)
	{
		growUPBig();
	}
	else if (nGrowUp == FireBig)
	{
		growUPFireBig();
	}
}