#pragma once
#include "Block.hpp"

class Entity;
class StaticBlock : public Block
{
    public:
        StaticBlock(Type type, sf::Vector2f position);
        // virtual void applyNormal(SceneNode& graph);
        virtual void handleBottomCollision(Entity& player);
        virtual void handleTopCollision(Entity& entity);
    private:
};