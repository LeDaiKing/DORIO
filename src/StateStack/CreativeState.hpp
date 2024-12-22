#pragma once
#include "State.hpp"
#include "StateStack.hpp"
#include "../Player.hpp"    
#include "../World/World.hpp"
#include <string>

class CreativeState : public State{
    public:
        CreativeState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
};