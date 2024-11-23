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
    nHitBox = sf::Vector2f(35.f, 25.f);
    nSpeed = sf::Vector2f(128.f, 0.f);
    nMaxVelocity = sf::Vector2f(128.f, 0.f);
    nJumpVelocitty = 0;
}

unsigned int Enemy::getCategory() const
{
    return Category::Enemy;
}

void Enemy::attackPlayer(Dough& player)
{
    sf::FloatRect playerHitBox = player.getHitBox();
    sf::FloatRect enemyHitBox = getHitBox();

    collision::Side side = checkCollisionSide(playerHitBox, enemyHitBox);

    if (side != collision::None)
    {
        player.getDamage();
    }
    if (side == collision::Left)
    {
        player.addVelocity(-256, 0.f);
    }
    else if (side == collision::Right)
    {
        player.addVelocity(256, 0.f);
    }
    else if (side == collision::Top)
    {
        if (player.getPosition().x < getPosition().x)
            player.addVelocity(-256, -100);
        else
            player.addVelocity(256, -100);
    }
    else if (side == collision::Bottom)
    {
        player.addVelocity(0.f, 256);
    }
}