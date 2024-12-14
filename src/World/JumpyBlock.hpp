#pragma once
#include "StaticBlock.hpp"


class JumpyBlock : public StaticBlock
{
    public:
        JumpyBlock(Type type, sf::Vector2f position);
        virtual void handleTopCollision(Entity& entity);
        void setJumpForce(int force);
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    private:
        int nJumpForce;
        bool nIsPressed;
        sf::Time nTimePressed;
        // float nSpeedSlide;
        // std::vector<sf::Vector2f> nPath;
        // int nCurrentPath;
        // int nDirLoop;
        // bool nIsLoop;
        // std::vector<Entity*> nEntities;
};