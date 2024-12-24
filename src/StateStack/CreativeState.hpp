#pragma once
#include "State.hpp"
#include "StateStack.hpp"
#include "../Player.hpp"    
#include "../World/World.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../UI/Container.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <string>

class CreativeState : public State{
    public:
        const int MAX_X = 2976;
        const int MAX_Y = 800;
        const int BLOCK_SIZE = 32;
        enum Cursor{
            Paint,
            Hand,
            Erase,
        };
        CreativeState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        sf::View nView;
        GUI::Container nBarControl; 
        GUI::Container nBarBlock;
        Cursor nCursor = Cursor::Paint;

};