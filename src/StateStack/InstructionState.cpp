#include "InstructionState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


InstructionState::InstructionState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, upButton(context, Textures::ID::UpButtonNormal, Textures::ID::UpButtonSelected, Textures::ID::UpButtonPressed)    
, leftButton(context, Textures::ID::LeftButtonNormal, Textures::ID::LeftButtonSelected, Textures::ID::LeftButtonPressed)
, rightButton(context, Textures::ID::RightButtonNormal, Textures::ID::RightButtonSelected, Textures::ID::RightButtonPressed)
, downButton(context, Textures::ID::DownButtonNormal, Textures::ID::DownButtonSelected, Textures::ID::DownButtonPressed)
, attackButton(context, Textures::ID::AttackButtonNormal, Textures::ID::AttackButtonSelected, Textures::ID::AttackButtonPressed)
, doubleUpButton(context, Textures::ID::DoubleUpButtonNormal, Textures::ID::DoubleUpButtonSelected, Textures::ID::DoubleUpButtonPressed)
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, nGUIContainerText()
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    upButton.setPosition(398, 91.37);
    upButton.setIsSelected(false);

    auto upText = std::make_shared<GUI::Label>("JUMP");
    upText->setPosition(650, 81.37);
    upText->setCharacterSize(30);
    nGUIContainerText.pack(upText);

    leftButton.setPosition(398, 181.37);
    leftButton.setIsSelected(false);

    auto leftText = std::make_shared<GUI::Label>("RUN LEFT");
    leftText->setPosition(650, 171.37);
    leftText->setCharacterSize(30);
    nGUIContainerText.pack(leftText);

    rightButton.setPosition(398, 271.37);
    rightButton.setIsSelected(false);

    auto rightText = std::make_shared<GUI::Label>("RUN RIGHT");
    rightText->setPosition(650, 261.37);
    rightText->setCharacterSize(30);
    nGUIContainerText.pack(rightText);

    downButton.setPosition(398, 361.37);
    downButton.setIsSelected(false);

    auto downText = std::make_shared<GUI::Label>("CROUCH");
    downText->setPosition(650, 351.37);
    downText->setCharacterSize(30);
    nGUIContainerText.pack(downText);

    attackButton.setPosition(405.265, 451.37);
    attackButton.setIsSelected(false);

    auto attackText = std::make_shared<GUI::Label>("SHOOT");
    attackText->setPosition(650, 441.37);
    attackText->setCharacterSize(30);
    nGUIContainerText.pack(attackText);

    doubleUpButton.setPosition(408.0, 541.37);
    doubleUpButton.setIsSelected(false);

    auto doubleUpText = std::make_shared<GUI::Label>("DOUBLE JUMP");
    doubleUpText->setPosition(650, 531.37);
    doubleUpText->setCharacterSize(30);
    nGUIContainerText.pack(doubleUpText);

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
    });
}

void InstructionState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(nBackgroundSprite);
    window.draw(backButton);
    window.draw(upButton);
    window.draw(leftButton);
    window.draw(rightButton);
    window.draw(downButton);
    window.draw(attackButton);
    window.draw(doubleUpButton);
    window.draw(nGUIContainerText);
}

bool InstructionState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();

    return true;
}

bool InstructionState::handleEvent(const sf::Event& event)
{
    backButton.handleEvent(event);
    return false;
}
