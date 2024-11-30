#include "TitleState.hpp"

TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, nText()
, nameTitle()
, nShowText(true)
, nTextEffectTime(sf::Time::Zero) {
    this->nBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
    this->nText.setFont(context.fonts->get(Fonts::Main));
    this->nText.setString("Press any key to start");
    centerOrigin(this->nText);
    this->nText.setPosition(context.window->getView().getSize() / 2.f);

    this->nameTitle.setFont(context.fonts->get(Fonts::Main));
    this->nameTitle.setString("DORIO");
    this->nameTitle.setCharacterSize(100);
    this->nameTitle.setFillColor(sf::Color::White);
    this->nameTitle.setOutlineColor(sf::Color::Black);
    this->nameTitle.setOutlineThickness(5.f);
    centerOrigin(this->nameTitle);
    this->nameTitle.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y / 4.f);
}   

void TitleState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(nBackgroundSprite);
    window.draw(this->nameTitle);
    if (this->nShowText) {
        window.draw(this->nText);
    }
}

bool TitleState::update(sf::Time dt) {
    this->nTextEffectTime += dt;
    if (this->nTextEffectTime >= sf::seconds(0.5f)) {
        this->nShowText = !this->nShowText;
        this->nTextEffectTime = sf::Time::Zero;
    }
    return true;
}

bool TitleState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        this->requestStackPop();
        this->requestStackPush(States::ID::Game);
    }
    return true;
}

