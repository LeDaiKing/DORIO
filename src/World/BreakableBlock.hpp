#pragma once
#include "Block.hpp"
#include "Animation.hpp"
class Dough;
class BreakableBlock : public Block
{
    public:
        BreakableBlock(sf::Vector2f position);
        // virtual ~BreakableBlock();
    protected:
        virtual void updateCurrent(sf::Time dt);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        
    public:
        virtual void applyNormal(SceneNode& graph);

        void breakBlock(Dough& player);

    private:
        bool nIsBroken;
        Animation nBreakAnimation;
        int nStateMove;
        float nMoveSpeed;
        sf::Vector2f nOriginPosition;
        sf::Vector2f nDestination;

};