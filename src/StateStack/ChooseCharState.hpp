#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../UI/Container.hpp"
#include "../UI/Button.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ChooseCharState : public State{
    public:
        ChooseCharState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::Sprite nBackgroundSprite;
        sf::Sprite nChar, nCharIntro;
        GUI::Container nGUIContainer;
        GUI::Button backButton;
        GUI::Button previousButton;
        GUI::Button nextButton;
        GUI::Button saveButton;
        GUI::Button instructionButton;
        GUI::Button chooseModeButton;
};