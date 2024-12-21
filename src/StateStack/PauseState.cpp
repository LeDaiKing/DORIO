#include "PauseState.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include "State.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../UI/Button.hpp"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, rectButton(context, GUI::Button::Type::RectangleButton)
, nBar()
{
    rectButton.setPosition(600.5, 400);
    rectButton.setIsSelected(false);

    auto homeButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::HomeButton);
    homeButton->setPosition(386, 400);
    homeButton->setCallback([this] ()
    {
        requestStackClear();
        requestStackPush(States::Title);
    });

    auto saveButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::SaveButton);
    saveButton->setPosition(600, 400);
    saveButton->setCallback([this] ()
    {
        // save game
    });

    auto startButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::StartButton);
    startButton->setPosition(814, 400); 
    startButton->setCallback([this] ()
    {
        requestStackPop();
    });


    nBar.pack(homeButton);
    nBar.pack(saveButton);
    nBar.pack(startButton);
    nBar.selectNext();
    nBar.selectNext();
}

void PauseState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(rectButton);
    window.draw(this->nBar);
}

bool PauseState::update(sf::Time dt) {
    return false;
}

bool PauseState::handleEvent(const sf::Event& event) {
    nBar.handleEvent(event);
    return false;
}

