#include "ChooseCharState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"

ChooseCharState::ChooseCharState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, GUI::Button::Type::BackButton)
{

}
/*
void ChooseCharState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
}

bool ChooseCharState::update(sf::Time dt)
{
    return true;
}

bool ChooseCharState::handleEvent(const sf::Event& event)
{
    nGUIContainer.handleEvent(event);
    backButton.handleEvent(event);
    return false;
}
*/