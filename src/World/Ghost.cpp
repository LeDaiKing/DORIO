#include "Ghost.hpp"
#include "Dough.hpp"
#include "World.hpp"
#include <iostream>
#include "Projectile.hpp"
#include "../ConfigLoader.hpp"

Ghost::Ghost(Type type, sf::Vector2f position)
: Enemy(type, position)
// , nGSpeed(400.f)
// , nMaxGVelocity(96.f)
// , nFireRange(200.f)
// , nFireSpeed(96.f)
// , nFireCooldown(sf::seconds(1.5f))
, nCurFireCooldown(sf::Time::Zero)
, nTarget(sf::Vector2f(0.f, 0.f))
// , nTimeDisappear(sf::seconds(8.f))
{
    setUpEntity();
    setAnimationState(State::Idle);
}

void Ghost::setUpEntity()
{
    addAnimationState(State::Idle, 107, 10, sf::seconds(1.f), sf::Vector2i(44, 30), true);
    addAnimationState(State::Appear, 0, 4, sf::seconds(0.5), sf::Vector2i(44, 30), false);
    addAnimationState(State::Disappear, 30, 4, sf::seconds(0.5), sf::Vector2i(44, 30), false);
    addAnimationState(State::Invisible, 60, 4, sf::seconds(0.5), sf::Vector2i(16, 16), true);
    addAnimationState(State::Hit, 76, 5, sf::seconds(0.5), sf::Vector2i(44, 30), false);
    addAnimationState(State::Dead, 76, 5, sf::seconds(0.5), sf::Vector2i(44, 30), false);
    nSprite.turnInverse();
   
    const nlohmann::json& config = ConfigLoader::getInstance().getConfig("Enemy/Ghost");
   
    nHitBox = toVector2<float>(config["HitBox"]);
    nGSpeed = config["GSpeed"];
    nMaxGVelocity = config["MaxGVelocity"];
    nFireRange = config["FireRange"];
    nFireSpeed = config["FireSpeed"];
    nFireCooldown = sf::seconds(config["FireCoolDown"]);
    nJumpVelocity = config["JumpVelocity"];
    nTimeDisappear = sf::seconds(config["TimeDisappear"]);
    // nSpeed = sf::Vector2f(400.f, 400.f);
    // nMaxVelocity = sf::Vector2f(96.f, 96.f);
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
    if (nCurrentState != State::Dead)
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
    

        nTimeDisappear -= dt;
        if (nCurrentState == State::Appear)
        {
            if (nSprite.isFinished())
            {
                nCurrentState = State::Idle;
                nSprite.setAnimationState(nCurrentState);
            }
        }

        if (nTimeDisappear <= sf::Time::Zero)
        {
            nTimeDisappear = sf::seconds(8.f);
            nCurrentState = State::Appear;
            nSprite.setAnimationState(nCurrentState);
        }
        else if (nTimeDisappear <= sf::seconds(3.f))
        {
            setAnimationState(State::Disappear);
            if (nSprite.isFinished())
            {
                nCurrentState = State::Invisible;
                nSprite.setAnimationState(nCurrentState);
            }
        }
    }
        
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
            if (nTarget.x < getPosition().x && nDirection == false)
            {
                nDirection = true;
                nSprite.setFlipped(true);
            }
            else if (nTarget.x > getPosition().x && nDirection == true)
            {
                nDirection = false;
                nSprite.setFlipped(false);
            }
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

void Ghost::isTargetInRange(const sf::Vector2f& target)
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

void Ghost::setFireCooldown(sf::Time time)
{
    nFireCooldown = time;
}

void Ghost::setFireRange(float range)
{
    nFireRange = range;
}

void Ghost::setFireSpeed(float speed)
{
    nFireSpeed = speed;
}

void Ghost::setAnimationState(State type)
{
    if (type != State::Dead && (nCurrentState == State::Appear || nCurrentState == State::Disappear || nCurrentState == State::Invisible))
        return;

    Entity::setAnimationState(type);
}

void Ghost::getDamage(int damage)
{
    if (nCurrentState == State::Invisible)
        return;
    Entity::getDamage(damage);
}