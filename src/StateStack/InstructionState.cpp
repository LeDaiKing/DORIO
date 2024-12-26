#include "InstructionState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


InstructionState::InstructionState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, instructionButton(context, Textures::ID::InstructionButtonNormal, Textures::ID::InstructionButtonSelected, Textures::ID::InstructionButtonPressed)
, musicButton(context, Textures::ID::MusicButtonNormal, Textures::ID::MusicButtonSelected, Textures::ID::MusicButtonPressed)
, nGUIContainerText()
, nGUIContainerButton()
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    auto upButton = std::make_shared<GUI::Button>(context, Textures::ID::UpButtonNormal, Textures::ID::UpButtonSelected, Textures::ID::UpButtonPressed);
    upButton->setPosition(398, 91.37);
    upButton->setIsSelected(false);
    nGUIContainerButton.pack(upButton);

    auto upText = std::make_shared<GUI::Label>("JUMP");
    upText->setPosition(650, 81.37);
    upText->setCharacterSize(30);
    nGUIContainerText.pack(upText);

    auto leftButton = std::make_shared<GUI::Button>(context, Textures::ID::LeftButtonNormal, Textures::ID::LeftButtonSelected, Textures::ID::LeftButtonPressed);
    leftButton->setPosition(398, 181.37);
    leftButton->setIsSelected(false);
    nGUIContainerButton.pack(leftButton);


    auto leftText = std::make_shared<GUI::Label>("RUN LEFT");
    leftText->setPosition(650, 171.37);
    leftText->setCharacterSize(30);
    nGUIContainerText.pack(leftText);

    auto rightButton = std::make_shared<GUI::Button>(context, Textures::ID::RightButtonNormal, Textures::ID::RightButtonSelected, Textures::ID::RightButtonPressed);
    rightButton->setPosition(398, 271.37);
    rightButton->setIsSelected(false);
    nGUIContainerButton.pack(rightButton);

    auto rightText = std::make_shared<GUI::Label>("RUN RIGHT");
    rightText->setPosition(650, 261.37);
    rightText->setCharacterSize(30);
    nGUIContainerText.pack(rightText);

    auto downButton = std::make_shared<GUI::Button>(context, Textures::ID::DownButtonNormal, Textures::ID::DownButtonSelected, Textures::ID::DownButtonPressed);
    downButton->setPosition(398, 361.37);
    downButton->setIsSelected(false);
    nGUIContainerButton.pack(downButton);

    auto downText = std::make_shared<GUI::Label>("CROUCH");
    downText->setPosition(650, 351.37);
    downText->setCharacterSize(30);
    nGUIContainerText.pack(downText);

    auto attackButton = std::make_shared<GUI::Button>(context, Textures::ID::AttackButtonNormal, Textures::ID::AttackButtonSelected, Textures::ID::AttackButtonPressed);
    attackButton->setPosition(405.265, 451.37);
    attackButton->setIsSelected(false);
    nGUIContainerButton.pack(attackButton);

    auto attackText = std::make_shared<GUI::Label>("SHOOT");
    attackText->setPosition(650, 441.37);
    attackText->setCharacterSize(30);
    nGUIContainerText.pack(attackText);

    auto doubleUpButton = std::make_shared<GUI::Button>(context, Textures::ID::DoubleUpButtonNormal, Textures::ID::DoubleUpButtonSelected, Textures::ID::DoubleUpButtonPressed);
    doubleUpButton->setPosition(408.0, 541.37);
    doubleUpButton->setIsSelected(false);
    nGUIContainerButton.pack(doubleUpButton);

    auto doubleUpText = std::make_shared<GUI::Label>("DOUBLE JUMP");
    doubleUpText->setPosition(650, 531.37);
    doubleUpText->setCharacterSize(30);
    nGUIContainerText.pack(doubleUpText);

    instructionButton.setPosition({75, 92});
    instructionButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Instruction);
    });

    musicButton.setPosition({75, 221});
    musicButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Music);
    });

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
    window.draw(nGUIContainerText);
    window.draw(nGUIContainerButton);
    window.draw(instructionButton);
    window.draw(musicButton);

}

bool InstructionState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();

    if (instructionButton.isMouseOver(window))
        instructionButton.setSelectedSprite();
    else instructionButton.setNormalSprite();

    if (musicButton.isMouseOver(window))
        musicButton.setSelectedSprite();
    else musicButton.setNormalSprite();

    return true;
}

bool InstructionState::handleEvent(const sf::Event& event)
{
    backButton.handleEvent(event);
    musicButton.handleEvent(event);
    instructionButton.handleEvent(event);
    return false;
}
