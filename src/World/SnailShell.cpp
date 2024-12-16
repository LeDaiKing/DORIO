#include "SnailShell.hpp"
#include "Dough.hpp"
#include <iostream>
#include "../ConfigLoader.hpp"

SnailShell::SnailShell(Type type, sf::Vector2f position)
: Enemy(type, position)
, nPush(false)
{
    setUpEntity();
    setAnimationState(State::Idle);
}


void SnailShell::setUpEntity()
{
    addAnimationState(State::Idle, 0, 6, sf::seconds(0.6), sf::Vector2i(38, 24), true);
    // addAnimationState(State::Walk, 30, 10, sf::seconds(1), sf::Vector2i(38, 24), true);
    addAnimationState(State::Hit, 48, 4, sf::seconds(0.3), sf::Vector2i(38, 24), false);
    addAnimationState(State::Dead, 24, 5, sf::seconds(0.5), sf::Vector2i(38, 24), false);
    nSprite.turnInverse();

    const nlohmann::json& config = ConfigLoader::getInstance().getConfig("Enemy/SnailShell");

    nHitBox = toVector2<float>(config["HitBox"]);
    nSpeed = toVector2<float>(config["Speed"]);
    nMaxVelocity = toVector2<float>(config["MaxVelocity"]);
    nJumpVelocity = config["JumpVelocity"];
}

void SnailShell::attackPlayer(Dough& player)
{
    if (nCurrentState == State::Dead)
        return;
    if (nPush && abs(getVelocity().x) + 20 >= nMaxVelocity.x)
    {
        Enemy::attackPlayer(player);
        return;
    }
    nPush = true;
    if (player.getPosition().x < getPosition().x)
    {
        accelerate(nSpeed.x, 0);
    }
    else
    {
        accelerate(-nSpeed.x, 0);
    }
}

void SnailShell::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nCurrentState != State::Dead)
    {
        if (nPush)
        {
            if (getVelocity().x < 0)
            {
                accelerate(-nSpeed.x, 0);
            }
            else if (getVelocity().x > 0)
            {
                accelerate(nSpeed.x, 0);
            }
        
            if (abs(getVelocity().x) + 20 >= nMaxVelocity.x)
            {
                Command attackEnemyCmd;
                attackEnemyCmd.category = Category::Enemy;
                attackEnemyCmd.action = derivedAction<Enemy>([this] (Enemy& enemy, sf::Time)
                {
                    if (enemy.getType() != Type::SnailShell)
                        attackEnemy(enemy);
                });
                commands.push(attackEnemyCmd);
            }

            if (nCurrentState == State::Hit && nSprite.isFinished())
            {
                setAnimationState(State::Idle);
            }

        }
    }
    Enemy::updateCurrent(dt, commands);
}

bool SnailShell::isPushed()
{
    return nPush;
}

void SnailShell::attackEnemy(Enemy& enemy)
{
    if (getBoundingRect().intersects(enemy.getBoundingRect()))
    {
        enemy.getDamage(1);
    }
}

void SnailShell::hitBlock()
{
    if (!nPush) 
    {
        setVelocity(0.f, getVelocity().y);
    }
    
    setVelocity(-getVelocity().x, getVelocity().y);
    nDirection = !nDirection;
    nSprite.setFlipped(nDirection);
    setAnimationState(State::Hit);
}