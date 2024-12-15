#include "Snail.hpp"
#include "Utility.hpp"
#include "SnailShell.hpp"
#include <iostream>

Snail::Snail(Type type, sf::Vector2f position)
: Enemy(type, position)
, nOutShell(false)
{
    setUpEntity();
    setAnimationState(State::Idle);
}

void Snail::setUpEntity()
{
    addAnimationState(State::Idle, 24, 15, sf::seconds(0.8), sf::Vector2i(38, 24), true);
    addAnimationState(State::Walk, 72, 10, sf::seconds(0.8), sf::Vector2i(38, 24), true);
    addAnimationState(State::Hit, 0, 5, sf::seconds(0.5), sf::Vector2i(38, 24), false);
    addAnimationState(State::Dead, 0, 5, sf::seconds(0.5), sf::Vector2i(38, 24), false);
    addAnimationState(State::WithoutShell, 48, 1, sf::seconds(1), sf::Vector2i(38, 24), true);
    nSprite.turnInverse();
    nHitBox = sf::Vector2f(25.f, 20.f);
    nSpeed = sf::Vector2f(576.f, 0.f);
    nMaxVelocity = sf::Vector2f(64.f, 64.f);
    nJumpVelocity = 0;
}

void Snail::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nCurrentState == State::Dead && nSprite.isFinished() && !nOutShell)
    {
        nOutShell = true;
        // nCurrentState = State::WithoutShell;
        nSprite.setAnimationState(State::WithoutShell);
        Command outshell;
        outshell.category = Category::EnemyScene;
        outshell.action = [this] (SceneNode& node, sf::Time dt)
        {
            std::unique_ptr<class SnailShell> shell(new class SnailShell(Enemy::SnailShell, getPosition()));
            node.attachChild(std::move(shell));
        };
        commands.push(outshell);
    }

    Enemy::updateCurrent(dt, commands);
}

void Snail::setAnimationState(State type)
{
    if (nSprite.getCurrentAnimationID() == State::WithoutShell)
        return;
    Enemy::setAnimationState(type);
}