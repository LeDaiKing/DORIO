#include "TransitionState.hpp"
#include <iostream> 

TransitionState::TransitionState(StateStack& stack, Context context)
: State(stack, context)
, background()
, nClock()
{
    nClock.restart();
}

void TransitionState::draw() { 
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    background.setSize(window.getView().getSize());
    float percentage = nClock.getElapsedTime().asMilliseconds() / TIME;
    background.setFillColor(sf::Color(0, 0, 0, 255 - 255 * percentage));
    window.draw(background);
}

bool TransitionState::update(sf::Time dt) {
    if (nClock.getElapsedTime().asMilliseconds() > TIME) {
        requestStackPop();
    }
    return false;
}

bool TransitionState::handleEvent(const sf::Event& event) {
    return false;
}
