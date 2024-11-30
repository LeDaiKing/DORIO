#pragma once
#include "State.hpp"
#include "StateStack.hpp"
#include "../Player.hpp"    
#include "../World/World.hpp"

class GameState : public State{
    public:
        GameState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
    private:
        World nWorld;
        Player& nPlayer;
};