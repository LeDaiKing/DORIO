#pragma once
#include "Block.hpp"

class Entity;
class StaticBlock : public Block
{
    public:
        StaticBlock(Type type, sf::Vector2f position);
        // virtual void applyNormal(SceneNode& graph);
        virtual void handleBottomCollision(Dough& player);
        virtual void handleTopCollision(Entity& entity);
    private:
};