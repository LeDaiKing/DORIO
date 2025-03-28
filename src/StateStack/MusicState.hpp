#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../UI/Container.hpp"
#include "../UI/Button.hpp"
#include "../UI/VolumeBar.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"
#include <vector>



#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MusicState : public State{
    public:
        MusicState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
        
    private:
        int nSelectedSlot = -1;
        sf::Sprite nBackgroundSprite;
        GUI::Button backButton;
        GUI::Button instructionButton;
        GUI::Button musicButton;
        GUI::Container nGUIContainerText;
        GUI::VolumeBar nMusicBar;
        GUI::VolumeBar nSoundBar;
        // GUI::Button startButton;
        // GUI::Button deleteButton;
        // GUI::Button resetButton;
};