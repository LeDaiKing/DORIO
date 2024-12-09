#include "Ghost.hpp"
#include "Dough.hpp"
#include "World.hpp"
#include <iostream>
#include "Projectile.hpp"

Ghost::Ghost(Type type, sf::Vector2f position)
: Enemy(type, position)
, nGSpeed(400.f)
, nMaxGVelocity(96.f)
, nFireRange(200.f)
, nFireSpeed(96.f)
, nFireCooldown(sf::seconds(1.5f))
, nCurFireCooldown(sf::Time::Zero)
, nTarget(sf::Vector2f(0.f, 0.f))
{
    setUpEntity();
    setAnimationState(State::Idle);
}

void Ghost::setUpEntity()
{
    addAnimationState(State::Idle, 0, 4, sf::seconds(0.5), sf::Vector2i(32, 32), true);
    // addAnimationState(State::Walk, 30, 10, sf::seconds(1), sf::Vector2i(44, 30), true);
    addAnimationState(State::Hit, 64, 4, sf::seconds(0.3), sf::Vector2i(32, 32), false);
    addAnimationState(State::Dead, 32, 8, sf::seconds(0.8), sf::Vector2i(32, 32), false);
    nHitBox = sf::Vector2f(25.f, 25.f);
    // nSpeed = sf::Vector2f(400.f, 400.f);
    // nMaxVelocity = sf::Vector2f(96.f, 96.f);
    nJumpVelocity = 0;
}

void Ghost::attackPlayer(Dough& player)
{
    if (nCurrentState == State::Dead)
        return;
    player.getDamage(1);
    if (player.getPosition().x < getPosition().x)
    {
        player.setVelocity(-512.f, 0);
    }
    else
    {
        player.setVelocity(512.f, 0);
    }
}

void Ghost::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    accelerate(0.f, -World::getGravity());
    if (nAIState == AIState::Firing)
    {
        if (nCurFireCooldown <= sf::Time::Zero)
        {
            fire(commands);
            nCurFireCooldown = nFireCooldown;
            Entity::updateCurrent(dt, commands);
            return;
        }
        setAIState(nPreAIState);
    }
    if (nCurFireCooldown > sf::Time::Zero)  
        nCurFireCooldown -= dt;
    
    Enemy::updateCurrent(dt, commands);
}

void Ghost::moveToPosition(sf::Time dt)
{

    if (length(nDestination - getPosition()) < 3.f)
    {
        setPosition(nDestination);
        setVelocity(0.f, 0.f);
        setAIState(AIState::None);
        return;
    }
    // std::cout << "Move to position" << std::endl;

    sf::Vector2f direction = nDestination - getPosition();
    // std::cout << direction.x << " " << direction.y << std::endl;
    direction = direction / length(direction);
    // std::cout << direction.x << " " << direction.y << std::endl;
    nSpeed = direction * nGSpeed;
    nMaxVelocity = sf::Vector2f(abs(direction.x) * nMaxGVelocity, abs(direction.y) * nMaxGVelocity);

    if (direction.x < 0 && nDirection == false)
    {
        nDirection = true;
        nSprite.setFlipped(true);
    }
    else if (direction.x > 0 && nDirection == true)
    {
        nDirection = false;
        nSprite.setFlipped(false);
    }
    float velocity = length(getVelocity());
    float firtion =length(0.8f * nSpeed);
    if (length(nDestination - getPosition()) > 0.5f * velocity * velocity / firtion)
    {
        // setVelocity(0.f, 0.f);
        accelerate(nSpeed);
    }

    setAnimationState(State::Walk);

}


void Ghost::fire(CommandQueue& commands)
{
    if (nTarget != sf::Vector2f(0.f, 0.f))
    {
        Command fire;
        fire.category = Category::ItemScene;
        fire.action = [this, &commands] (SceneNode& node, sf::Time dt)
        {
            std::unique_ptr<Projectile> projectile(new Projectile(Projectile::Type::FireBall, getPosition()));
            projectile->setSpeed(nFireSpeed);
            projectile->setTargetCategory(Category::PlayerDough);
            projectile->setTargetPosition(nTarget);
            node.attachChild(std::move(projectile));
        };
        commands.push(fire);
    }
    nTarget = sf::Vector2f(0.f, 0.f);
    setAIState(nPreAIState);
}

void Ghost::isTargetInRange(sf::Vector2f target)
{
    if (length(target - getPosition()) > nFireRange) return;
    if (nTarget == sf::Vector2f(0.f, 0.f))
    {
        nTarget = target;
    }
    else if (length(target - getPosition()) < length(nTarget - getPosition()))
    {
        nTarget = target;
    }
    nPreAIState = nAIState;
    setAIState(AIState::Firing);
}