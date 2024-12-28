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
, player1Button(context, Textures::ID::Player1ButtonNormal, Textures::ID::Player1ButtonSelected, Textures::ID::Player1ButtonPressed)
, player2Button(context, Textures::ID::Player2ButtonNormal, Textures::ID::Player2ButtonSelected, Textures::ID::Player2ButtonPressed)
, nGUIContainerText()
, nGUIContainerButton()
, nGUIContainerButton1()
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    auto upButton = std::make_shared<GUI::Button>(context, Textures::ID::UpButtonNormal, Textures::ID::UpButtonSelected, Textures::ID::UpButtonPressed);
    upButton->setPosition(398, 91.37);
    upButton->setIsSelected(false);
    nGUIContainerButton.pack(upButton);

    auto upButton1 = std::make_shared<GUI::Button>(context, Textures::ID::UpButtonNormal1, Textures::ID::UpButtonSelected1, Textures::ID::UpButtonPressed1);
    upButton1->setPosition(398, 91.37);
    upButton1->setIsSelected(false);
    nGUIContainerButton1.pack(upButton1);

    auto upText = std::make_shared<GUI::Label>("JUMP");
    upText->setPosition(650, 81.37);
    upText->setCharacterSize(30);
    nGUIContainerText.pack(upText);

    auto leftButton = std::make_shared<GUI::Button>(context, Textures::ID::LeftButtonNormal, Textures::ID::LeftButtonSelected, Textures::ID::LeftButtonPressed);
    leftButton->setPosition(398, 181.37);
    leftButton->setIsSelected(false);
    nGUIContainerButton.pack(leftButton);

    auto leftButton1 = std::make_shared<GUI::Button>(context, Textures::ID::LeftButtonNormal1, Textures::ID::LeftButtonSelected1, Textures::ID::LeftButtonPressed1);
    leftButton1->setPosition(398, 181.37);
    leftButton1->setIsSelected(false);
    nGUIContainerButton1.pack(leftButton1);


    auto leftText = std::make_shared<GUI::Label>("RUN LEFT");
    leftText->setPosition(650, 171.37);
    leftText->setCharacterSize(30);
    nGUIContainerText.pack(leftText);

    auto rightButton = std::make_shared<GUI::Button>(context, Textures::ID::RightButtonNormal, Textures::ID::RightButtonSelected, Textures::ID::RightButtonPressed);
    rightButton->setPosition(398, 271.37);
    rightButton->setIsSelected(false);
    nGUIContainerButton.pack(rightButton);

    auto rightButton1 = std::make_shared<GUI::Button>(context, Textures::ID::RightButtonNormal1, Textures::ID::RightButtonSelected1, Textures::ID::RightButtonPressed1);
    rightButton1->setPosition(398, 271.37);
    rightButton1->setIsSelected(false);
    nGUIContainerButton1.pack(rightButton1);

    auto rightText = std::make_shared<GUI::Label>("RUN RIGHT");
    rightText->setPosition(650, 261.37);
    rightText->setCharacterSize(30);
    nGUIContainerText.pack(rightText);

    auto downButton = std::make_shared<GUI::Button>(context, Textures::ID::DownButtonNormal, Textures::ID::DownButtonSelected, Textures::ID::DownButtonPressed);
    downButton->setPosition(398, 361.37);
    downButton->setIsSelected(false);
    nGUIContainerButton.pack(downButton);

    auto downButton1 = std::make_shared<GUI::Button>(context, Textures::ID::DownButtonNormal1, Textures::ID::DownButtonSelected1, Textures::ID::DownButtonPressed1);
    downButton1->setPosition(398, 361.37);
    downButton1->setIsSelected(false);
    nGUIContainerButton1.pack(downButton1);


    auto downText = std::make_shared<GUI::Label>("CROUCH");
    downText->setPosition(650, 351.37);
    downText->setCharacterSize(30);
    nGUIContainerText.pack(downText);

    auto attackButton = std::make_shared<GUI::Button>(context, Textures::ID::AttackButtonNormal, Textures::ID::AttackButtonSelected, Textures::ID::AttackButtonPressed);
    attackButton->setPosition(405.265, 451.37);
    attackButton->setIsSelected(false);
    nGUIContainerButton.pack(attackButton);

    auto attackButton1 = std::make_shared<GUI::Button>(context, Textures::ID::AttackButtonNormal1, Textures::ID::AttackButtonSelected1, Textures::ID::AttackButtonPressed1);   
    attackButton1->setPosition(405.265, 451.37);
    attackButton1->setIsSelected(false);
    nGUIContainerButton1.pack(attackButton1);

    auto attackText = std::make_shared<GUI::Label>("SHOOT");
    attackText->setPosition(650, 441.37);
    attackText->setCharacterSize(30);
    nGUIContainerText.pack(attackText);

    auto doubleUpButton = std::make_shared<GUI::Button>(context, Textures::ID::DoubleUpButtonNormal, Textures::ID::DoubleUpButtonSelected, Textures::ID::DoubleUpButtonPressed);
    doubleUpButton->setPosition(408.0, 541.37);
    doubleUpButton->setIsSelected(false);
    nGUIContainerButton.pack(doubleUpButton);

    auto doubleUpButton1 = std::make_shared<GUI::Button>(context, Textures::ID::DoubleUpButtonNormal1, Textures::ID::DoubleUpButtonSelected1, Textures::ID::DoubleUpButtonPressed1);
    doubleUpButton1->setPosition(408.0, 541.37);
    doubleUpButton1->setIsSelected(false);
    nGUIContainerButton1.pack(doubleUpButton1);

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

    player1Button.setPosition({75, 351});
    player1Button.setCallback([this] ()
    {
        player = 2;
    });

    player2Button.setPosition({75, 351});
    player2Button.setCallback([this] ()
    {
        player = 1;
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
    if (player == 1) window.draw(nGUIContainerButton);
    else window.draw(nGUIContainerButton1);
    window.draw(instructionButton);
    window.draw(musicButton);
    if (player == 1) window.draw(player1Button);
    else window.draw(player2Button);
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

    if (player == 1) {
        if (player1Button.isMouseOver(window))
            player1Button.setSelectedSprite();
        else player1Button.setNormalSprite();
    }
    else {
        if (player2Button.isMouseOver(window))
            player2Button.setSelectedSprite();
        else player2Button.setNormalSprite();
    }

    return true;
}

bool InstructionState::handleEvent(const sf::Event& event)
{
    backButton.handleEvent(event);
    musicButton.handleEvent(event);
    instructionButton.handleEvent(event);
    if (player == 1) player1Button.handleEvent(event);
    else player2Button.handleEvent(event);
    return false;
}
