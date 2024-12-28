#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../UI/Container.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"
#include "../Music/SoundPlayer.hpp"
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class LosingState : public State{
    public:
        LosingState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
        virtual void saveCurrentState();
    private:
        sf::Sprite nBackgroundSprite;
        GUI::Button leaderboardButton;
        GUI::Button homeButton;
        GUI::Button retryButton;
        SoundPlayer& nSoundPlayer;
};