#include "SceneNode.hpp"
#include "Animation.hpp"

class CheckPoint: public SceneNode
{
    public:
        enum Type
        {
            Start,
            Checkpoint,
            End,
        };
    public:
        CheckPoint(Type type, sf::Vector2f position);
        virtual sf::FloatRect getBoundingRect() const;
        // virtual unsigned int getCategory() const;

    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        Animation nSprite;
        Type nType;
};