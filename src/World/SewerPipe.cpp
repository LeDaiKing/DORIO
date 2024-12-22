#include "SewerPipe.hpp"
#include "Category.hpp" 
#include <SFML/Graphics/Rendertarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "World.hpp"
#include <iostream> 
SewerPipe::SewerPipe(Type type, sf::Vector2f position, int height)
: StaticBlock(type, position)
, nPipe(TextureHolder::getInstance().get(Textures::Pipe))
, nHeight(height)
, nCanGetIn(false)
, nOutPipe(nullptr)
, nPlayerIn(nullptr)
, nPlayerOut(nullptr)
, nSpeedGetIn(32.f)
{
    centerOrigin(nPipe);
    // nPipe.setPosition(position.x, position.y + 32);
}

// void SewerPipe::handleTopCollision(Entity& entity)
// {
//     if (!nCanGetIn)
//     {
//         StaticBlock::handleTopCollision(entity);
//         return;
//     }
//     Dough& player = static_cast<Dough&>(entity);
//     nPlayerIn = &player;
// }

void SewerPipe::applyNormal(SceneNode& graph)
{
    if (!(graph.getCategory() & Category::PlayerDough)) 
    {
        StaticBlock::applyNormal(graph);
        return;
    }
    
   if (nPlayerIn != nullptr || nPlayerOut != nullptr) return;
    StaticBlock::applyNormal(graph);
}

sf::FloatRect SewerPipe::getBoundingRect() const
{
    sf::FloatRect bound = StaticBlock::getBoundingRect();
    bound.height += (nHeight - 1) * 32;
    return bound;
}

void SewerPipe::setOutPipe(SewerPipe* pipe)
{
    std::cout << "Set Out Pipe" << std::endl;
    nOutPipe = pipe;
    nCanGetIn = true;
}

void SewerPipe::outPlayer(Dough* player)
{
    if (nOutPipe == nullptr)
    {
        return;
    }
    nPlayerOut = player;
    nPlayerOut->setPosition(getWorldPosition().x, getWorldPosition().y + nPlayerOut->getBoundingRect().height / 2 - 16);
}

bool SewerPipe::inPlayer(Dough* player)
{
    if (!nCanGetIn || nPlayerIn != nullptr)
    {
        return false;
    }
    nPlayerIn = player;
    nPlayerIn->setMotionless(true);
    return true;
}

void SewerPipe::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nPlayerIn != nullptr)
    {
        nPlayerIn->move(0.f, nSpeedGetIn * dt.asSeconds());
        if (getBoundingRect().top < nPlayerIn->getBoundingRect().top)
        {
            nOutPipe->outPlayer(nPlayerIn);
            nPlayerIn = nullptr;
        }
    }
    if (nPlayerOut != nullptr)
    {
        nPlayerOut->move(0.f, -nSpeedGetIn * dt.asSeconds());
        // nPlayerOut->accelerate(0.f, -World::getGravity());
        if (getBoundingRect().top > nPlayerOut->getBoundingRect().top + nPlayerOut->getBoundingRect().height + 5)
        {
            nPlayerOut->setMotionless(false);
            nPlayerOut = nullptr;
        }
    }
    StaticBlock::updateCurrent(dt, commands);
}

void SewerPipe::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    // target.draw(nPipe, states);
    for (int i = 1; i < nHeight; i++)
    {
        sf::Sprite pipe = nPipe;
        pipe.setPosition(0, i * 32);   
        target.draw(pipe, states);
    }
    StaticBlock::drawCurrent(target, states);
}

void SewerPipe::save(std::ofstream& file)
{
    StaticBlock::save(file);
    file.write(reinterpret_cast<const char*>(&nHeight), sizeof(nHeight));
}