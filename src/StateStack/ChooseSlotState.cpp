#include "ChooseSlotState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


ChooseSlotState::ChooseSlotState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainerSlot()
, nGUIContainerConfirm()
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, instructionButton(context, Textures::ID::InstructionButtonNormal, Textures::ID::InstructionButtonSelected, Textures::ID::InstructionButtonPressed)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));
    
    auto slotButton1 = std::make_shared<GUI::Button>(context, Textures::ID::SlotButtonNormal, Textures::ID::SlotButtonSelected, Textures::ID::SlotButtonPressed);
    slotButton1->setPosition(373, 400.5);
    slotButton1->setText("NEW !");
    slotButton1->setCallback([this] ()
    {
        nSelectedSlot = 1;
    });

    auto slotButton2 = std::make_shared<GUI::Button>(context, Textures::ID::SlotButtonNormal, Textures::ID::SlotButtonSelected, Textures::ID::SlotButtonPressed);
    slotButton2->setPosition(632, 400.5);
    slotButton2->setText("NEW !");
    slotButton2->setCallback([this] ()
    {
        nSelectedSlot = 2;
    });

    auto slotButton3 = std::make_shared<GUI::Button>(context, Textures::ID::SlotButtonNormal, Textures::ID::SlotButtonSelected, Textures::ID::SlotButtonPressed);
    slotButton3->setPosition(891, 400.5);
    slotButton3->setText("NEW !");
    slotButton3->setCallback([this] ()
    {
        nSelectedSlot = 3;
    });

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Title);
    });

    instructionButton.setPosition({75, 92});
    instructionButton.setIsSelected(false);
    instructionButton.setCallback([this] ()
    {
        requestStackPush(States::Instruction);
    });

    auto startButton = std::make_shared<GUI::Button>(context, Textures::ID::StartButtonNormal, Textures::ID::StartButtonSelected, Textures::ID::StartButtonPressed);    
    startButton->setPosition(373, 615);
    startButton->setCallback([this] ()
    {
        std::ofstream file("file/CurSave/save.txt");
        file << "file/Save" + std::to_string(nSelectedSlot);
        file.close();
        requestStackPop();
        requestStackPush(States::ID::ChoosePlayer);
    });

    auto deleteButton = std::make_shared<GUI::Button>(context, Textures::ID::DeleteButtonNormal, Textures::ID::DeleteButtonSelected, Textures::ID::DeleteButtonPressed);
    deleteButton->setPosition({891, 615});
    deleteButton->setCallback([this] ()
    {
        // delete file
        clearFolder("file/Save" + std::to_string(nSelectedSlot));
    });

    auto resetButton = std::make_shared<GUI::Button>(context, Textures::ID::ResetButtonNormal, Textures::ID::ResetButtonSelected, Textures::ID::ResetButtonPressed);
    resetButton->setPosition({632, 615});
    resetButton->setCallback([this] ()
    {
        nSelectedSlot = -1;
        nGUIContainerConfirm.selectPrevious();
    });
    // pass


    nGUIContainerSlot.pack(slotButton1);
    nGUIContainerSlot.pack(slotButton2);
    nGUIContainerSlot.pack(slotButton3);

    nGUIContainerConfirm.pack(resetButton);
    nGUIContainerConfirm.pack(deleteButton);
    nGUIContainerConfirm.pack(startButton);
    nGUIContainerConfirm.selectNext();
    nGUIContainerConfirm.selectNext();
}

void ChooseSlotState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(nBackgroundSprite);
    window.draw(nGUIContainerSlot);
    window.draw(backButton);
    window.draw(instructionButton);
    if (nSelectedSlot != -1)
        window.draw(nGUIContainerConfirm);
}

bool ChooseSlotState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();
    if (instructionButton.isMouseOver(window))
        instructionButton.setSelectedSprite();
    else instructionButton.setNormalSprite();
    return true;
}

bool ChooseSlotState::handleEvent(const sf::Event& event)
{
    if (nSelectedSlot != -1) {
        nGUIContainerConfirm.handleEvent(event);
    }
    else {
        nGUIContainerSlot.handleEvent(event);
    }
    backButton.handleEvent(event);
    instructionButton.handleEvent(event);
    return false;
}
