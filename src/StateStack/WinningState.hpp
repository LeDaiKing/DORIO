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
#include <vector>



#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class WinningState : public State{
    public:
        WinningState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        const float speed = 50;
        const float secondToScore = 1;
        const float hpToScore = 10;
        const float DELAY = 1;
        int nSelectedSlot = -1;
        sf::Sprite nBackgroundSprite;
        sf::Sprite nCharacterSprite;
        GUI::Button startButton;
        GUI::Button leaderboardButton;
        sf::Sprite nClock;
        sf::Sprite nHeart;
        GUI::Label textScore;
        GUI::Label textClock;
        GUI::Label textHeart;
        sf::Sprite winBadge;
        sf::Sprite coinBadge;
        bool isEnd = false;
        float nSecond = 0;
        float nHP = 0;
        float nScore = 0;
        float delaySecond = 0;
        float delayHP = 0;
        float delayUpdate = 0;
        float alphaWin = 0;
        float alphaCoin = 0;
        // GUI::Button startButton;
        // GUI::Button deleteButton;
        // GUI::Button resetButton;
};