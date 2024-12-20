#include "ChooseSlotState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


ChooseSlotState::ChooseSlotState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainerSlot()
, nGUIContainerConfirm()
, backButton(context, GUI::Button::Type::BackButton)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));
    
    auto slotButton1 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SlotButton);
    slotButton1->setPosition(373, 400.5);
    slotButton1->setText("NEW !");
    slotButton1->setCallback([this] ()
    {
        nSelectedSlot = 1;
    });

    auto slotButton2 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SlotButton);
    slotButton2->setPosition(632, 400.5);
    slotButton2->setText("NEW !");
    slotButton2->setCallback([this] ()
    {
        nSelectedSlot = 2;
    });

    auto slotButton3 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SlotButton);
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

    auto startButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::StartButton);
    startButton->setPosition(373, 615);
    startButton->setCallback([this] ()
    {
        std::ofstream file("file/CurSave/save.txt");
        file << "file/Save" + std::to_string(nSelectedSlot);
        file.close();
        requestStackPop();
        requestStackPush(States::ID::ChoosePlayer);
    });

    auto deleteButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::DeleteButton);
    deleteButton->setPosition({891, 615});
    deleteButton->setCallback([this] ()
    {
        // delete file
    });

    auto resetButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::ResetButton);
    resetButton->setPosition({632, 615});
    resetButton->setCallback([this] ()
    {
        nSelectedSlot = -1;
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
    if (nSelectedSlot != -1)
        window.draw(nGUIContainerConfirm);
}

bool ChooseSlotState::update(sf::Time dt)
{
    if (backButton.isMouseOver(*getContext().window))
        backButton.setIsSelected(true);
    else
        backButton.setIsSelected(false);
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
    return false;
}
