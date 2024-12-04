#include "Dough.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Category.hpp"
#include "Enemy.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

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
, Entity(TextureHolder::getInstance().get(toTextureID(type)))
{
	setUpEntity();
	setAnimationState(State::Idle);
}

// void Dough::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
// {
// }

unsigned int Dough::getCategory() const
{
	switch (nType)
	{
		case Dough1:
		case Dough2:
			return Category::PlayerDough;

		default:
			return Category::PlayerDough;
	}
}

void Dough::updateCurrent(sf::Time dt)
{
	if (nTimeDamage > sf::Time::Zero)
	{
		nTimeDamage -= dt;
		if (nTimeDamage <= sf::Time::Zero)
		{
			if (nCurrentState == State::Hit)
				setAnimationState(State::Idle);
		}
	}

	if (nOnGround) stateJump = 0;
	
	
	Entity::updateCurrent(dt);
}

void Dough::setUpEntity()
{
	switch (nType)
	{
	case Dough1:
	case Dough2:
		nHitBox = sf::Vector2f(25.f, 28.f);
		nSpeed = sf::Vector2f(1024.f, 128.f);
		nMaxVelocity = sf::Vector2f(200.f, 1024.f);
		// friction = sf::Vector2f(0.f, 0.f);
		nJumpVelocity = 320;
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
	
}

void Dough::jump()
{
	if (stateJump == 0)
	{
		if (nCurrentState == State::Fall)
		{
			setAnimationState(State::DoubleJump);
			addVelocity(0.f, -getVelocity().y);
			addVelocity(0.f, -nJumpVelocity);
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
		addVelocity(0.f, -getVelocity().y);
		addVelocity(0.f, -nJumpVelocity2);
		stateJump++;
	} 
}

void Dough::attackEnemy(Enemy& enemy)
{
	enemy.getDamage();
}

void Dough::getDamage()
{
	setAnimationState(State::Hit);
	nTimeDamage = sf::seconds(0.35f);
}

void Dough::handleCollisionEnemies(SceneNode& graph)
{
	if (nCurrentState == State::Hit)
		return;
	
	if (graph.getCategory() & Category::Enemy)
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
	}

	for (Ptr& child : graph.getChildren())
    {
        handleCollisionEnemies(*child);
    }
}