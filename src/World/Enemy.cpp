#include "Enemy.hpp"
#include "Dough.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Category.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <iostream>
Textures::ID toTextureID(Enemy::Type type)
{
    switch (type)
    {
        case Enemy::CockRoach:
            return Textures::CockRoach;
        case Enemy::Ghost:
            return Textures::Ghost;
        case Enemy::Chicken:
            return Textures::Chicken;
        case Enemy::Snail:
            return Textures::Snail;
        case Enemy::SnailShell:
            return Textures::SnailShell;
    }
    return Textures::CockRoach;
}

Enemy::Enemy(Type type, sf::Vector2f position)
: Entity(TextureHolder::getInstance().get(toTextureID(type)))
, nType(type)
, nAIState(None)
, nCurBehavior(0)
, nDestination(position)
, nDirLoop(1)
{
    setPosition(position);
}


unsigned int Enemy::getCategory() const
{
    return Category::Enemy;
}


Enemy::Type Enemy::getType()
{
    return nType;
}

void Enemy::setAIState(AIState state)
{
    nAIState = state;
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    Entity::updateCurrent(dt, commands);

    if (nCurrentState == State::Dead) return;

    if (isAutoAI()) 
    {
        autoMove();
        return;
    }

    switch (nAIState)
    {
    case Patrolling:
        moveToPosition(dt);
        break;
    case Turning:
        turn();
        break;
    case Waiting:
        wait(dt);
        break;
    
    case None:
        if (nBehaviors.size() == 0) break;

        nBehaviors[nCurBehavior]();
        nCurBehavior += nDirLoop;
        if (nCurBehavior == nBehaviors.size())
        {
            nCurBehavior = nBehaviors.size() - 1;
            nDirLoop = -1;
        }
        else if (nCurBehavior == -1)
        {
            nCurBehavior = 0;
            nDirLoop = 1;
        }
        break;
    default:
        break;
    }
}

void Enemy::addMoveBehavior(sf::Vector2f offset)
{
    // sf::Vector2f temp = nDestination + offset;
    nBehaviors.push_back([this, offset] ()
    {
        sf::Vector2f noffset = offset;
        noffset.x *= nDirLoop;
        noffset.y *= nDirLoop;
        nDestination = getPosition() + noffset;
        setAIState(Patrolling);
    });

    nBeList.push_back({0, {offset.x, offset.y}});  
    // nDestination = temp;
}

void Enemy::addWaitBehavior(sf::Time time)
{
    nBehaviors.push_back([this, time] ()
    {
        nWaitingTime = time;
        setAIState(Waiting);
    });

    nBeList.push_back({1, {time.asSeconds(), 0}});
}

void Enemy::addTurnBehavior()
{
    nBehaviors.push_back([this] ()
    {
        setAIState(Turning);
    });

    nBeList.push_back({2, {0, 0}});
}

void Enemy::moveToPosition(sf::Time dt)
{   
    // std::cout << length(nDestination - getPosition()) << std::endl;
    // int sign = nDestination.x < getPosition().x ? -1 : 1;

    if (length(nDestination - getPosition()) < 3.f)
    {
        setVelocity(0.f, 0.f);
        nAcceleration = {0.f, 0.f};
        setPosition(nDestination);
        setAIState(None);
        return;
    }
    
    float velocity = length(getVelocity());
    float firtion = length(0.8f * nSpeed);
    
    if (length(nDestination - getPosition()) > 0.5f * velocity * velocity / firtion)
    {
         if (nDestination.x < getPosition().x)  walk(true);
         else walk(false);
    }
}

void Enemy::wait(sf::Time time)
{
    nWaitingTime -= time;
    if (nWaitingTime <= sf::Time::Zero)
    {
        nWaitingTime = sf::Time::Zero;
        setAIState(None);
    }
}

void Enemy::turn()
{
    nDirection = !nDirection;
    nSprite.setFlipped(nDirection);
    setAIState(None);
}

void Enemy::isTargetInRange(const sf::Vector2f& target)
{
    //
}

void Enemy::attackPlayer(Dough& player)
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


sf::FloatRect Enemy::getBoundingRect() const
{
    sf::FloatRect bound = getWorldTransform().transformRect(nSprite.getGlobalBounds());
	// sf::Vector2f pos = {bound.left + bound.width / 2 - nHitBox.x / 2, bound.top + bound.height - nHitBox.y};
	// sf::Vector2f pos = getPosition() - sf::Vector2f(nHitBox.x / 2, nHitBox.y / 2);
    bound.top -= (nHitBox.y - bound.height);
    bound.left -= (nHitBox.x - bound.width) / 2;
    bound.height = nHitBox.y;
    bound.width = nHitBox.x;

	return bound;
}

bool Enemy::isAutoAI()
{
    return (int)nBehaviors.size() == 0 && nAIState == None;
}

void Enemy::autoMove()
{
    walk(nDirection);
}

void Enemy::addBehavior(int type, float x, float y)
{
    switch (type)
    {
    case 0:
        addMoveBehavior(sf::Vector2f(x, y));
        break;
    case 1:
        addWaitBehavior(sf::seconds(x));
        break;
    case 2:
        addTurnBehavior();
        break;
    default:
        break;
    }
}

void Enemy::save(std::ofstream& file)
{
    if (nCurrentState == State::Dead) 
    {
        int type = -1;
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        return;
    }
    file.write(reinterpret_cast<const char*>(&nType), sizeof(nType));
    sf::Vector2f pos = getPosition();
    file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));

    file.write(reinterpret_cast<const char*>(&nCurBehavior), sizeof(nCurBehavior));
    file.write(reinterpret_cast<const char*>(&nDirLoop), sizeof(nDirLoop));
    int size = nBeList.size(); 
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (auto& be : nBeList)
    {
        file.write(reinterpret_cast<const char*>(&be.first), sizeof(be.first));
        file.write(reinterpret_cast<const char*>(&be.second.first), sizeof(be.second.first));
        file.write(reinterpret_cast<const char*>(&be.second.second), sizeof(be.second.second));
    }
}

void Enemy::load(std::ifstream& file)
{
    file.read(reinterpret_cast<char*>(&nCurBehavior), sizeof(nCurBehavior));
    file.read(reinterpret_cast<char*>(&nDirLoop), sizeof(nDirLoop));
    int size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    for (int i = 0; i < size; ++i)
    {
        int type;
        float x, y;
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        file.read(reinterpret_cast<char*>(&y), sizeof(y));
        addBehavior(type, x, y);
    }
}