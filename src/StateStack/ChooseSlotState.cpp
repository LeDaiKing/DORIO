#include "ChooseSlotState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"


ChooseSlotState::ChooseSlotState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, GUI::Button::Type::BackButton)
, saveButton(context, GUI::Button::Type::saveButton)
, startButton(context, GUI::Button::Type::StartButton)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));
    
    auto slotButton1 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SlotButton);
    slotButton1->setPosition(373, 400.5);
    slotButton1->setText("NEW !");
    slotButton1->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ID::ChoosePlayer);
    });

    auto slotButton2 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SlotButton);
    slotButton2->setPosition(632, 400.5);
    slotButton2->setText("NEW !");
    slotButton2->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ID::ChoosePlayer);
    });

    auto slotButton3 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SlotButton);
    slotButton3->setPosition(891, 400.5);
    slotButton3->setText("NEW !");
    slotButton3->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ID::ChoosePlayer);
    });

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Title);
    });

    saveButton.setPosition({500, 620});
    saveButton.setIsSelected(false);
    saveButton.setCallback([this] ()
    {
        // save file 
    });

    startButton.setPosition(759, 620);
    startButton.setIsSelected(false);
    startButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ID::ChooseMode);
    });
    

    nGUIContainer.pack(slotButton1);
    nGUIContainer.pack(slotButton2);
    nGUIContainer.pack(slotButton3);
}

void ChooseSlotState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
    window.draw(saveButton);
    window.draw(startButton);
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
    nGUIContainer.handleEvent(event);
    backButton.handleEvent(event);
    saveButton.handleEvent(event);
    return false;
}
