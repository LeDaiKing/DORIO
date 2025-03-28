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
#include "SewerPipe.hpp"
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

Textures::ID toTextureBigID(Dough::Type type)
{
	switch (type)
	{
		case Dough::Dough1:
			return Textures::BigDough1;
		case Dough::Dough2:
			return Textures::BigDough2;

	}
	return Textures::Dough1;
}

Textures::ID toTextureFireID(Dough::Type type)
{
	switch (type)
	{
		case Dough::Dough1:
			return Textures::FireBigDough1;
		case Dough::Dough2:
			return Textures::FireBigDough2;

	}
	return Textures::Dough1;
}

Dough::Dough(Type type, SoundPlayer& sounds)
: nType(type)
, nGrowUp(Small)
, Entity(TextureHolder::getInstance().get(toTextureID(type)))
, nBig(TextureHolder::getInstance().get(toTextureBigID(type)))
, nFireBig(TextureHolder::getInstance().get(toTextureFireID(type)))
, stateJump(0)
, nCoinsCount(0)
, nScore(0)
, nMotionless(false)
, nPipe(nullptr)
, nHit(0)
, isPlayer2(false)
, nSoundPlayer(sounds)
{
	centerOrigin(nBig);
	centerOrigin(nFireBig);

	setUpEntity();
	setAnimationState(State::Idle);
}

// void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
// {
// }

unsigned int Dough::getCategory() const
{
	if (!isPlayer2) return Category::PlayerDough1;
	return Category::PlayerDough2;
}

void Dough::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (nMotionless) 
	{
		nClosestBottomBlock = nullptr;
		nClosestTopBlock = nullptr;
		nAcceleration = sf::Vector2f(0.f, 0.f);
		nVelocity = sf::Vector2f(0.f, 0.f);
	}

	if ((nClosestTopBlock != nullptr))
	{	
		if ((nClosestTopBlock->getType() == Block::Type::SewerPipe))
		nPipe = static_cast<SewerPipe*>(nClosestTopBlock);
		else nPipe = nullptr;
	}

	if (!nOnGround) nPipe = nullptr;

	if (nTimeDamage > sf::Time::Zero)
	{
		nTimeDamage -= dt;
		nHit = (nHit + 1 )%30;
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


	// dough
	addAnimationState(State::Idle, 96, 12, sf::seconds(1.f), sf::Vector2i(32, 32), true);
	addAnimationState(State::Walk, 0, 4, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
	addAnimationState(State::Jump, 32, 5, sf::seconds(0.35f), sf::Vector2i(32, 32), false);
	addAnimationState(State::DoubleJump, 32, 5, sf::seconds(0.35f), sf::Vector2i(32, 32), false);
	addAnimationState(State::Fall, 64, 5, sf::seconds(0.35f), sf::Vector2i(32, 32), false);
	addAnimationState(State::Dead, 128,8, sf::seconds(0.7f), sf::Vector2i(32, 32), false);


	// Big dough
	nBig.addAnimationState(State::Idle, 0, 7, sf::seconds(1.f), sf::Vector2i(32, 64), true);
	nBig.addAnimationState(State::Walk, 128, 4, sf::seconds(0.5f), sf::Vector2i(32, 64), true);
	nBig.addAnimationState(State::Jump, 64, 8, sf::seconds(0.4f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::DoubleJump, 128, 4, sf::seconds(0.4f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::Fall, 192, 5, sf::seconds(0.5f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::Appear, 320, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);
	nBig.addAnimationState(State::Disappear, 256, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);	
	nBig.addAnimationState(State::Sit, 384, 1, sf::seconds(0.5f), sf::Vector2i(32, 32), true);


	// FireBig dough
	nFireBig.addAnimationState(State::Idle, 0, 7, sf::seconds(1.f), sf::Vector2i(32, 64), true);
	nFireBig.addAnimationState(State::Walk, 64, 6, sf::seconds(0.5f), sf::Vector2i(32, 64), true);
	nFireBig.addAnimationState(State::Jump, 128, 5, sf::seconds(0.3f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::DoubleJump, 128, 5, sf::seconds(0.3f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::Fall, 192, 1, sf::seconds(0.5f), sf::Vector2i(32, 64), false);	
	nFireBig.addAnimationState(State::Fire, 256, 6, sf::seconds(0.4f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::Appear, 384, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::Disappear, 320, 8, sf::seconds(0.7f), sf::Vector2i(32, 64), false);
	nFireBig.addAnimationState(State::Sit, 448, 1, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
}

void Dough::jump()
{
	// if (nPipe != nullptr && nPipe->isInvert())
	// {
	// 	if (nPipe->inPlayer(this))
	// 	{
	// 		std::cout << "In Pipe" << std::endl;
	// 		nPipe = nullptr;
	// 		return;
	// 	}
	// }
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
		setAnimationState(State::DoubleJump);
		// addVelocity(0.f, -getVelocity().y);
		setVelocity(getVelocity().x, -nJumpVelocity);
		stateJump++;
	} 
	nSoundPlayer.play(SoundEffect::jump);
}

void Dough::attackEnemy(Enemy& enemy)
{
	nSoundPlayer.play(SoundEffect::land);
	enemy.getDamage(1);
	if (enemy.getHitPoints() == 0) nScore += 100;
}

void Dough::getDamage(int damage)
{
	// setAnimationState(State::Hit);
	nSoundPlayer.play(SoundEffect::hurt);
	if (nTimeDamage > sf::Time::Zero)
		return;
	
	nTimeDamage = sf::seconds(3.f);
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
	if (nCurrentState != State::Dead && nHitPoints == 0) setAnimationState(State::Dead);
}

void Dough::handleCollisionEnemies(SceneNode& graph)
{
	if (nTimeDamage > sf::Time::Zero)
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
			if (item.getType() == Item::Type::Coin)
			{
				nSoundPlayer.play(SoundEffect::coin);
			}
			else if (item.getType() == Item::Type::Heart || item.getType() == Item::Type::Big || item.getType() == Item::Type::FireBig)
			{
				nSoundPlayer.play(SoundEffect::item);
			}
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
	if (nPipe != nullptr && !nPipe->isInvert())
	{
		if (nPipe->inPlayer(this))
		{
			std::cout << "In Pipe" << std::endl;
			nPipe = nullptr;
			return;
		}
	}


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
	nSoundPlayer.play(SoundEffect::powerup);
	addHitPoints(1);
	if (nGrowUp == Big || nGrowUp == FireBig) return;
	nGrowUp = Big;
	nCurrentState = State::Appear;
	nBig.setAnimationState(State::Appear);
}

void Dough::growUPFireBig()
{
	nSoundPlayer.play(SoundEffect::powerup);
	addHitPoints(1);
	if (nGrowUp == FireBig) return;
	nGrowUp = FireBig;
	nCurrentState = State::Appear;
	nFireBig.setAnimationState(State::Appear);
}

void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (nTimeDamage > sf::Time::Zero && nHit < 12)
		return;
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
	// target.draw(rect);

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
		if (nFireBig.getCurrentAnimationID() == State::Sit) fireBall->move(0.f, 10.f);
		fireBall->setTargetCategory(Category::Enemy);
		fireBall->setSpeed(312.f);
		if (nDirection) fireBall->setTargetDirection(sf::Vector2f(-1.f, 0.f));
		else fireBall->setTargetDirection(sf::Vector2f(1.f, 0.f));
		node.attachChild(std::move(fireBall));
		nSoundPlayer.play(SoundEffect::shoot);
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
	nScore += 10;
}
int Dough::getCoinsCount() const
{
	return nCoinsCount;
}

int Dough::getScore() const {
	return nScore;
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

void Dough::setMotionless(bool flag)
{
	nMotionless = flag;
}

bool Dough::isMarkedForRemoval() const
{
	return nCurrentState == State::Dead && nSprite.isFinished();
}
void Dough::setPlayer2(bool flag)
{
	isPlayer2= flag;
}

void Dough::setPlayer2(Dough* player2)
{
	
	nPlayer2 = player2;
}