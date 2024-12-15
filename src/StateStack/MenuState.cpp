#include "MenuState.hpp"
#include "../UI/Button.hpp"

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context) 
, nBackgroundSprite()
{
    auto playButton = std::make_shared<GUI::Button>(
        context, GUI::Button::Type::RectangleButton);
    playButton->setPosition(100, 250);
    playButton->setText("Play");
    playButton->setCallback([this] () {
        requestStackPop();
        requestStackPush(States::Game);
    });
    nGUIContainer.pack(playButton);
}

void MenuState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
}

bool MenuState::update(sf::Time dt) {
    return true;
}

bool MenuState::handleEvent(const sf::Event& event) {
    nGUIContainer.handleEvent(event);
    return false;
}