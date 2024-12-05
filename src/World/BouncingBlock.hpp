#pragma once
#include "Block.hpp"

class Dough;
class BouncingBlock : public Block
{
    public:
        BouncingBlock(Type type, sf::Vector2f position);
        virtual void applyNormal(SceneNode& graph);
        // virtual ~BreakableBlock();
        
    protected:
        virtual void updateCurrent(sf::Time dt);

    protected:
        int nStateBounce;
    private:
        float nBounceSpeed;
        sf::Vector2f nOriginPosition;
        sf::Vector2f nDestination;

};