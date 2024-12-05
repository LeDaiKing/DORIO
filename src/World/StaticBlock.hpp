#pragma once
#include "Block.hpp"


class StaticBlock : public Block
{
    public:
        StaticBlock(Type type, sf::Vector2f position);
        virtual void applyNormal(SceneNode& graph);
    private:
};