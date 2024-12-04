#pragma once
#include "Block.hpp"
#include "Animation.hpp"

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

        // void breakBlock();

    private:
        bool nIsBroken;
        Animation nBreakAnimation;

};