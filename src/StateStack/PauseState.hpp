#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State{
    public:
        PauseState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::Sprite nBackgroundSprite;
        sf::Text nPausedText;
        sf::Text nInstructionText;
};