#include "TitleState.hpp"

TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, nText()
, nameTitle()
, nShowText(true)
, nTextEffectTime(sf::Time::Zero) {
    this->nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::TitleScreen));
    this->nText.setFont(FontHolder::getInstance().get(Fonts::Main));
    this->nText.setString("Press any key to start");
    centerOrigin(this->nText);
    this->nText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 8.f * 7.f);

    this->nameTitle.setFont(FontHolder::getInstance().get(Fonts::Main));
    this->nameTitle.setString("DORIO");
    this->nameTitle.setCharacterSize(100);
    this->nameTitle.setFillColor(sf::Color::White);
    this->nameTitle.setOutlineColor(sf::Color::Black);
    this->nameTitle.setOutlineThickness(5.f);
    centerOrigin(this->nameTitle);
    this->nameTitle.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y / 4.f);
    context.music->play(Music::ID::BackgroundTheme);
    context.music->setVolume(2.0f / 3.0f * 100);
    context.sounds->setVolume(2.0f / 3.0f * 100);
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
        this->requestStackPush(States::ID::ChooseSlot);
    }
    return true;
}

