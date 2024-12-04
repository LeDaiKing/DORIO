#include "Enemy.hpp"
#include "Dough.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Category.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Enemy::Enemy()
: Entity(TextureHolder::getInstance().get(Textures::Enemy))
{
    setUpEntity();
    setAnimationState(State::Idle);
}

Enemy::Enemy(sf::Vector2f position)
: Entity(TextureHolder::getInstance().get(Textures::Enemy))
{
    setUpEntity();
    setAnimationState(State::Idle);
    setPosition(position);
}

void Enemy::setUpEntity()
{
    addAnimationState(State::Idle, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    addAnimationState(State::Walk, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    addAnimationState(State::Hit, 0, 5, sf::seconds(0.8), sf::Vector2i(44, 30), false);
    addAnimationState(State::Dead, 0, 5, sf::seconds(0.8), sf::Vector2i(44, 30), false);
    nHitBox = sf::Vector2f(35.f, 25.f);
    nSpeed = sf::Vector2f(128.f, 0.f);
    nMaxVelocity = sf::Vector2f(128.f, 0.f);
    nJumpVelocity = 0;
}

unsigned int Enemy::getCategory() const
{
    return Category::Enemy;
}

void Enemy::attackPlayer(Dough& player)
{
    if (nCurrentState == State::Dead)
        return;
    player.getDamage();
    if (player.getPosition().x < getPosition().x)
    {
        player.setVelocity(-512.f, 0);
    }
    else
    {
        player.setVelocity(512.f, 0);
    }
}

void Enemy::getDamage()
{
	if (nCurrentState == State::Hit || nCurrentState == State::Dead)
		return;

    // nTimeDamage = sf::seconds(0.5f);
	nHitPoints--;
	if (nHitPoints <= 0)
	{
		setAnimationState(State::Dead);
		// std::cout << "Dead" << std::endl;
	}
	else
	{
		setAnimationState(State::Hit);
	}
}