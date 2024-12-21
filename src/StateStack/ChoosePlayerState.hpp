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

class ChoosePlayerState : public State{
    public:
        ChoosePlayerState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::Sprite nBackgroundSprite;
        sf::Sprite handSprite;
        sf::Sprite choosePlayerDeco;
        GUI::Button backButton;
        GUI::Button instructionButton;
        // GUI::Button onePlayerButton;
        // GUI::Button twoPlayerButton;
        GUI::Container nGUIContainer;
        bool drawHand;
};