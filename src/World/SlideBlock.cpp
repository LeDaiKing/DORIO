#include "SlideBlock.hpp"
#include "Utility.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include <iostream>
#include "Dough.hpp"
#include "Entity.hpp"
// #include <iostream>


SlideBlock::SlideBlock(Type type, sf::Vector2f position)
: StaticBlock(type, position)
, nSpeedSlide(32.f)
, nCurrentPath(0)
, nIsLoop(true)
, nDirLoop(1)
{
    nPath.push_back(position);
}

void SlideBlock::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nPath.size() == 1 || nCurrentPath == nPath.size())
    {
        return;
    }
    sf::Vector2f velocity = getVelocity();
    move(velocity * dt.asSeconds());
    sf::Vector2f offest = velocity * dt.asSeconds();
    if (length(nPath[nCurrentPath + nDirLoop] - getPosition()) < nSpeedSlide * dt.asSeconds())
    {
        // offest += nPath[nCurrentPath + nDirLoop] - getPosition();
        if (nIsLoop)
        {
            nCurrentPath += nDirLoop;
            if (nCurrentPath == nPath.size() - 1 || nCurrentPath == 0)
            {
                nDirLoop *= -1;
            }
        }
        else
        {
            nCurrentPath++;
        }

        if (nCurrentPath != nPath.size())
        {
            setPosition(nPath[nCurrentPath]);
        }
    }

    // std::cout << nEntities.size() << std::endl;

    for (auto it = nEntities.begin(); it != nEntities.end();)
    {
        sf::FloatRect bound = (*it)->getBoundingRect();
        bound.height += 5.f;
        
        if (bound.intersects(getBoundingRect()))
        {
            // (*it)->setPosition((*it)->getPosition().x, getBoundingRect().top - bound.height / 2);
            // (*it)->setVelocity((*it)->getVelocity().x, 0.f);
            (*it)->move(offest);
            it++;
        }
        else
        {
            it = nEntities.erase(it);
        }
    }
}

void SlideBlock::addPath(sf::Vector2f offset)
{
    nPath.push_back(nPath.back() + offset);
}

void SlideBlock::setSpeedSlide(float speed)
{
    nSpeedSlide = speed;
}

void SlideBlock::setLoop(bool flag)
{
    nIsLoop = flag;
}

sf::Vector2f SlideBlock::getVelocity() const
{
    sf::Vector2f direction = nPath[nCurrentPath + nDirLoop] - nPath[nCurrentPath];
    float distance = length(direction);
    return direction / distance * nSpeedSlide;
}

void SlideBlock::handleTopCollision(Entity& entity)
{
    sf::FloatRect bound = getBoundingRect();
    entity.setPosition(entity.getPosition().x, bound.top - entity.getBoundingRect().height / 2);
    entity.setVelocity(entity.getVelocity().x, 0.f);
    entity.accelerate(0.f, -World::getGravity());
    entity.setOnGround(true);
    // entity.addVelocity(getVelocity());
    // entity.applyFriction(getVelocity());
    if (std::find(nEntities.begin(), nEntities.end(), &entity) == nEntities.end())
    nEntities.push_back(&entity);
    
}