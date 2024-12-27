#include "State.hpp"
#include "StateStack.hpp"
#include <iostream> 

State::State(StateStack& stack, Context context)
: nStack(&stack)
, nContext(context) {
}

State::~State() {
    sf::RenderWindow& window = *getContext().window;    
    window.setKeyRepeatEnabled(false);    
}

void State::saveCurrentState() {
    // pass
}

void State::requestStackPush(States::ID stateID) { nStack->pushState(stateID); }

void State::requestStackPop() {
    nStack->popState();
}

void State::requestStackClear() {
    nStack->clearStates();
}

State::Context State::getContext() const {
    return nContext;
}

State::Context::Context(sf::RenderWindow& window,
                        Player& player, MusicPlayer& music, 
                        SoundPlayer& sounds, ParallelTask& loadingTask)
: window(&window)
, player(&player) 
, music(&music) 
, sounds(&sounds) 
, loadingTask(&loadingTask) 
, saveFile(new std::string("file/")) {
}

