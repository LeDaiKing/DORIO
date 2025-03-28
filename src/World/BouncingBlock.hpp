#pragma once
#include "Block.hpp"

class Entity;
class BouncingBlock : public Block
{
    public:
        BouncingBlock(Type type, sf::Vector2f position);
        virtual void handleBottomCollision(Entity& player);
        virtual void handleTopCollision(Entity& player);
        // virtual void applyNormal(SceneNode& graph);
        // virtual ~BreakableBlock();
        
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

    protected:
        int nStateBounce;
        sf::Vector2f nOriginPosition;
    private:
        float nBounceSpeed;
        sf::Vector2f nDestination;

};