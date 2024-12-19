#pragma once
#include "StaticBlock.hpp"


class SlideBlock : public StaticBlock
{
    public:
        SlideBlock(Type type, sf::Vector2f position);
        // virtual void applyNormal(SceneNode& graph);
        virtual void handleTopCollision(Entity& entity);
        void addPath(sf::Vector2f offset);
        void setSpeedSlide(float speed);
        void setLoop(bool flag);
        sf::Vector2f getVelocity() const;
        virtual void load(std::ifstream& file);
        virtual void save(std::ofstream& file);
    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        // virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        float nSpeedSlide;
        std::vector<sf::Vector2f> nPath;
        int nCurrentPath;
        int nDirLoop;
        bool nIsLoop;
        std::vector<Entity*> nEntities;
};