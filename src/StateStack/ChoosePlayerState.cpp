#include "ChoosePlayerState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include "../UI/Component.hpp"
#include <iostream>

ChoosePlayerState::ChoosePlayerState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, handSprite()
, choosePlayerDeco()
, backButton(context, GUI::Button::Type::BackButton)
, instructionButton(context, GUI::Button::Type::instructionButton)
// , onePlayerButton(context, GUI::Button::Type::onePlayerButton)
// , twoPlayerButton(context, GUI::Button::Type::twoPlayerButton)
, drawHand(false)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseCharScreen));
    handSprite.setTexture(TextureHolder::getInstance().get(Textures::handSprite));
    choosePlayerDeco.setTexture(TextureHolder::getInstance().get(Textures::choosePlayerDeco));
    choosePlayerDeco.setPosition({189, 0});

    auto onePlayerButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::onePlayerButton);
    onePlayerButton->setPosition({418, 437});
    onePlayerButton->setIsSelected(true);
    onePlayerButton->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    auto twoPlayerButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::twoPlayerButton);
    twoPlayerButton->setPosition({872, 443});
    twoPlayerButton->setIsSelected(true);
    twoPlayerButton->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseSlot);
    });  

    instructionButton.setPosition({75, 92});
    instructionButton.setIsSelected(false);
    instructionButton.setCallback([this] ()
    {
        requestStackPush(States::Instruction);
    });

    nGUIContainer.pack(onePlayerButton);
    nGUIContainer.pack(twoPlayerButton);
}

void ChoosePlayerState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(choosePlayerDeco);
    window.draw(backButton);
    window.draw(instructionButton);
    window.draw(nGUIContainer);
    // window.draw(onePlayerButton);
    // window.draw(twoPlayerButton);
    if (drawHand)
        window.draw(handSprite);
}

bool ChoosePlayerState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();
    if (instructionButton.isMouseOver(window))
        instructionButton.setSelectedSprite();
    else instructionButton.setNormalSprite();
    int index = nGUIContainer.getSelectedIndex();
    auto selected = nGUIContainer.getSelectedChild();
    drawHand = true;
    // std::cerr << "Index: " << index << std::endl;
    assert(index != -1);
    if (index == 0) {
        handSprite.setPosition({445, 469});
    }
    else if (index == 1) {
        handSprite.setPosition({901, 466});
    }
    // if (!onePlayerButton.isMouseOver(window) && !twoPlayerButton.isMouseOver(window))
    //     drawHand = false;
    // else if (onePlayerButton.isMouseOver(window)) {
    //     handSprite.setPosition({445, 469});
    //     drawHand = true;
    // }
    // else if (twoPlayerButton.isMouseOver(window)) {
    //     handSprite.setPosition({901, 466});
    //     drawHand = true;
    // }
    return true;
}

bool ChoosePlayerState::handleEvent(const sf::Event& event)
{
    backButton.handleEvent(event);
    instructionButton.handleEvent(event);
    nGUIContainer.handleEvent(event);
    // onePlayerButton.handleEvent(event);
    // twoPlayerButton.handleEvent(event);
    return false;
}
