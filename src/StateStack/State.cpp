#include "State.hpp"
#include "StateStack.hpp"

State::State(StateStack& stack, Context context)
: nStack(&stack)
, nContext(context) {
}

State::~State() {
}

void State::requestStackPush(States::ID stateID) {
    nStack->pushState(stateID);
}

void State::requestStackPop() {
    nStack->popState();
}

void State::requestStackClear() {
    nStack->clearStates();
}

State::Context State::getContext() const {
    return nContext;
}

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures,
                        FontHolder& fonts, Player& player)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, player(&player) {
}
