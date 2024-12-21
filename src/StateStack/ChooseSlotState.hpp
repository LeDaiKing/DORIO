#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../UI/Container.hpp"
#include "../UI/Button.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"
#include <vector>



#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ChooseSlotState : public State{
    public:
        ChooseSlotState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        int nSelectedSlot = -1;
        sf::Sprite nBackgroundSprite;
        GUI::Container nGUIContainerSlot;
        GUI::Container nGUIContainerConfirm;
        GUI::Button backButton;
        GUI::Button instructionButton;
        // GUI::Button startButton;
        // GUI::Button deleteButton;
        // GUI::Button resetButton;
};