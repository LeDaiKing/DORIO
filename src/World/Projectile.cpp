#include "Projectile.hpp"
#include "Entity.hpp"
#include "Utility.hpp"
#include "BreakableBlock.hpp"
#include "Enemy.hpp"
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
// #include "../Command/Command.hpp"

Projectile::Projectile(Type type, sf::Vector2f position)
: Item(type, position)
, nSpeed(60.f)
, nDamage(1)
, nTargetCategory(0)
, nHitBox(20, 20)
, nTimeLife(sf::seconds(3))
{
    switch (type)
    {
    case Projectile::FireBall:
        nAnimation.addAnimationState(AniState::Fire, 0, 5, sf::seconds(0.5), sf::Vector2i(32, 32), true);
        nAnimation.addAnimationState(AniState::Explode, 32, 3, sf::seconds(0.2), sf::Vector2i(32, 32), false);
        break;
    default:
        break;
    }
    nAnimation.setAnimationState(AniState::Fire);
}

void Projectile::activate(Entity& player)
{
    if (nIsCollected) return;
    if (!(nTargetCategory & player.getCategory())) return;

    if (player.getAnimationState() == Entity::State::Dead || player.getAnimationState() == Entity::State::Hit) return;
    player.getDamage(nDamage);
    sf::Vector2f direction = player.getPosition() - getPosition();
    direction /= length(direction);
    if (direction.x < 0)
    {
        nAnimation.setFlipped(true);
        setRotation(getAngle(direction) + 180.f);
    }
    else
    {
        nAnimation.setFlipped(false);
        setRotation(getAngle(direction));
    }
    destroy();
}

void Projectile::setSpeed(float speed)
{
    nSpeed = speed;
}

void Projectile::setDamage(unsigned int damage)
{
    nDamage = damage;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (!isMarkedForRemoval())
    {

        if (nTargetCategory & Category::Enemy) 
        {
            handleEnemyCollision(commands);
        }

        handleBlockCollision(commands);

        if (!nIsCollected)
        {
            move(nVelocity * dt.asSeconds());
            nTimeLife -= dt;
            if (nTimeLife <= sf::Time::Zero)
            {
                destroy();
            }
        }
        nAnimation.update(dt);
        // if (nAniState == AniState::Explode
    }

}

void Projectile::setTargetCategory(unsigned int category)
{
    nTargetCategory = category;
}

void Projectile::setTargetDirection(sf::Vector2f direction)
{
    nVelocity = direction * nSpeed;

    float angle = getAngle(direction);
    if (direction.x < 0)
    {
        nAnimation.setFlipped(true);
        setRotation(angle + 180.f);
    }
    else
    {
        setRotation(angle);
    }
    // rotate(atan2(direction.y, direction.x) * 180.f / 3.14159265f);
    // rotate(atan2(direction.y, direction.x) * 180.f / 3.14159265f);
}

void Projectile::setTargetPosition(sf::Vector2f position)
{
    sf::Vector2f direction = position - getPosition();
    direction /= length(direction);
    setTargetDirection(direction);
}

void Projectile::destroy()
{
    nIsCollected = true;
    setAnimationState(AniState::Explode);
}

void Projectile::setAnimationState(AniState state)
{
    nAnimation.setAnimationState(state);
}

bool Projectile::isMarkedForRemoval() const
{
    return nIsCollected && nAnimation.isFinished();
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!isMarkedForRemoval())
    {
        sf::FloatRect bounds = getBoundingRect();
        sf::RectangleShape rect(sf::Vector2f(bounds.width, bounds.height));
        rect.setPosition(bounds.left, bounds.top);
        rect.setFillColor(sf::Color(255, 255, 255, 0));
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(1.f);
        // target.draw(rect);
        target.draw(nAnimation, states);
    }
}


sf::FloatRect Projectile::getBoundingRect() const
{
    sf::FloatRect bound = getWorldTransform().transformRect(nAnimation.getGlobalBounds());
	sf::Vector2f pos = getPosition() - sf::Vector2f(nHitBox.x / 2, nHitBox.y / 2);
	return sf::FloatRect(pos, nHitBox);
}

void Projectile::handleEnemyCollision(CommandQueue& commands)
{
    Command enemyHit;
    enemyHit.category = Category::Enemy;
    enemyHit.action = derivedAction<Enemy>([this] (Enemy& enemy, sf::Time)
    {
        if (enemy.getBoundingRect().intersects(getBoundingRect()))
        {
            // handleEnemyCollision(enemy);
            activate(enemy);
        }
    });
    commands.push(enemyHit);
}

void Projectile::handleBlockCollision(CommandQueue& commands)
{
    if (nIsCollected) return;
    Command blockHit;
    blockHit.category = Category::Block;
    blockHit.action = derivedAction<Block>([this] (Block& block, sf::Time)
    {
        if (block.getBoundingRect().intersects(getBoundingRect()))
        {
            destroy();
            nIsCollected = true;
            if (dynamic_cast<BreakableBlock*>(&block) != nullptr)
            {
                BreakableBlock& breakableBlock = static_cast<BreakableBlock&>(block);
                breakableBlock.breakBlock();
            }
        }
    });
    commands.push(blockHit);
}