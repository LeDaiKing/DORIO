#pragma once
#include "BouncingBlock.hpp"
#include "Animation.hpp"
class Dough;
class BreakableBlock : public BouncingBlock
{
    public:
        BreakableBlock(Type type, sf::Vector2f position);
        // virtual ~BreakableBlock();
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        
    public:
        virtual void handleBottomCollision(Dough& player);

        // void breakBlock(Dough& player);

        virtual bool isMarkedForRemoval() const;

    private:
        bool nIsBroken;
        Animation nBreakAnimation;
};