#pragma once
#include "StateStack.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State{
    public:
        TitleState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::Sprite nBackgroundSprite;
        sf::Text nText;
        sf::Text nameTitle;
        bool nShowText;
        sf::Time nTextEffectTime;
};