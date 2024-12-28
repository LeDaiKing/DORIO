#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../UI/VolumeBar.hpp"
#include "../Utility.hpp"
#include "../UI/Container.hpp"
#include "../UI/Button.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State{
    public:
        PauseState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        GUI::Button rectButton;
        GUI::Container nBar;
        GUI::VolumeBar nMusicBar;
        GUI::VolumeBar nSoundBar;
};