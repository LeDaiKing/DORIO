#pragma once
#include "State.hpp"
#include "StateStack.hpp"
#include "../Player.hpp"    
#include "../World/World.hpp"
#include <string>

class GameState : public State{
    public:
        GameState(StateStack& stack, Context context);
        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);
        virtual void saveCurrentState();
    private:
        World nWorld;
        Player& nPlayer;
        std::string saveFile;
};