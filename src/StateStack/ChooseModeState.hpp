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

class ChooseModeState : public State{
    public:
        ChooseModeState(StateStack& stack, Context context);
        virtual ~ChooseModeState();
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
        virtual void saveCurrentState();
    private:
        sf::Sprite nBackgroundSprite;
        GUI::Container nGUIContainer;
        GUI::Button backButton;
        // GUI::Button playStartButton;
        GUI::Button settingButton;
        //GUI::Button chooseCharButton;
        std::vector<sf::Sprite> kitchenModeBadge;
        std::vector<sf::Sprite> hallwayModeBadge;
        std::vector<sf::Sprite> gardenModeBadge;
};