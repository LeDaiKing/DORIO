#include "StaticBlock.hpp"
#include "Dough.hpp"

class SewerPipe: public StaticBlock
{
    public:
        SewerPipe(Type type, sf::Vector2f position, int height = 1, bool invert = false);
        // virtual void handleTopCollision(Entity& player);
        virtual void applyNormal(SceneNode& graph);
        void setOutPipe(SewerPipe* pipe);
        void outPlayer(Dough* player);
        bool inPlayer(Dough* player);

        sf::FloatRect getBoundingRect() const;
        virtual void save(std::ofstream& file);
        bool isInvert() const;
        // void setInPipe(SewerPipe* pipe);

    protected:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        bool nCanGetIn;
        int nHeight;
        sf::Sprite nPipe;
        SewerPipe* nOutPipe;
        Dough* nPlayerIn;
        Dough* nPlayerOut;
        float nSpeedGetIn;
        bool nInvert;
};