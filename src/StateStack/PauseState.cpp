#include "PauseState.hpp"
#include "..\Utility.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include "State.hpp"
#include "..\Holder\ResourceIdentifiers.hpp"
#include "..\Holder\ResourceHolder.hpp"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nPausedText()
, nInstructionText() {
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f viewSize = context.window->getView().getSize();

    this->nPausedText.setFont(font);
    this->nPausedText.setString("Game Paused");
    this->nPausedText.setCharacterSize(70);
    centerOrigin(this->nPausedText);
    this->nPausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

    this->nInstructionText.setFont(font);
    this->nInstructionText.setString("Press Back to return to the game");
    centerOrigin(this->nInstructionText);
    this->nInstructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(this->nPausedText);
    window.draw(this->nInstructionText);
}

bool PauseState::update(sf::Time dt) {
    return false;
}

bool PauseState::handleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) {
        return false;
    }

    if (event.key.code == sf::Keyboard::Escape) {
        requestStackPop();
    }

    if (event.key.code == sf::Keyboard::BackSpace) {
        requestStackClear();
        requestStackPush(States::ID::Menu);
    }

    return false;
}

