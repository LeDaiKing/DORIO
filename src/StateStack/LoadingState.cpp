#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"
#include "../StateStack/ParallelTask.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class LoadingState : public State{
    public:
        LoadingState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::Text nLoadingText;
        sf::RectangleShape nProgressBarBackground;  
        sf::RectangleShape nProgressBar;
        ParallelTask nLoadingTask;
};