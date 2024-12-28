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
        virtual void saveCurrentState();
    private:
        sf::Sprite nBackgroundSprite;
        sf::Text nStat;
        sf::Sprite nChar, nCharIntro, nStatBox;
        Animation nCharAni1;
        Animation nCharAni2;
        GUI::Container nGUIContainer;
        GUI::Button backButton;
        GUI::Button previousButton;
        GUI::Button nextButton;
        GUI::Button saveButton;
        GUI::Button settingButton;
        int nSpeed1, nJumpVelocity1;
        int nSpeed2, nJumpVelocity2;
        int selectedChar = 1;
};