#include "ChoosePlayerState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"

ChoosePlayerState::ChoosePlayerState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, handSprite()
, choosePlayerDeco()
, backButton(context, GUI::Button::Type::BackButton)
, onePlayerButton(context, GUI::Button::Type::onePlayerButton)
, twoPlayerButton(context, GUI::Button::Type::twoPlayerButton)
, drawHand(false)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));
    handSprite.setTexture(TextureHolder::getInstance().get(Textures::handSprite));
    choosePlayerDeco.setTexture(TextureHolder::getInstance().get(Textures::choosePlayerDeco));
    choosePlayerDeco.setPosition({189, 0});

    onePlayerButton.setPosition({418, 437});
    onePlayerButton.setIsSelected(false);
    onePlayerButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    twoPlayerButton.setPosition({872, 443});
    twoPlayerButton.setIsSelected(false);
    twoPlayerButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Title);
    });  

}

void ChoosePlayerState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(choosePlayerDeco);
    window.draw(backButton);
    window.draw(onePlayerButton);
    window.draw(twoPlayerButton);
    if (drawHand)
        window.draw(handSprite);
}

bool ChoosePlayerState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();
    if (!onePlayerButton.isMouseOver(window) && !twoPlayerButton.isMouseOver(window))
        drawHand = false;
    else if (onePlayerButton.isMouseOver(window)) {
        handSprite.setPosition({445, 469});
        drawHand = true;
    }
    else if (twoPlayerButton.isMouseOver(window)) {
        handSprite.setPosition({901, 466});
        drawHand = true;
    }
    return true;
}

bool ChoosePlayerState::handleEvent(const sf::Event& event)
{
    backButton.handleEvent(event);
    onePlayerButton.handleEvent(event);
    twoPlayerButton.handleEvent(event);
    return false;
}
