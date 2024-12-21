#include "ChooseCharState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <iostream>

ChooseCharState::ChooseCharState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, GUI::Button::Type::BackButton)
, previousButton(context, GUI::Button::Type::previousButton)
, nextButton(context, GUI::Button::Type::nextButton)
, saveButton(context, GUI::Button::Type::SaveButton)
, instructionButton(context, GUI::Button::Type::instructionButton)
, chooseModeButton(context, GUI::Button::Type::chooseModeButton)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseCharScreen));
    nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
    nChar.setPosition({376, 289});
    nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::charIntro));
    nCharIntro.setPosition({208, 53});

    auto charSlot1 = std::make_shared<GUI::Button>(context, GUI::Button::Type::charSlot1);
    charSlot1->setPosition({930, 131});
    charSlot1->setCallback([this] ()
    {
        nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
    });
    auto charSlot2 = std::make_shared<GUI::Button>(context, GUI::Button::Type::charSlot2);
    charSlot2->setPosition({1088, 131});
    charSlot2->setCallback([this] ()
    {
        nChar.setTexture(TextureHolder::getInstance().get(Textures::char2Sprite));
    });
    
    previousButton.setPosition({284.5, 482.5});
    previousButton.setIsSelected(false);
    previousButton.setCallback([this, charSlot1] ()
    {
        nGUIContainer.selectNext();
        if(nGUIContainer.getSelectedChild() == charSlot1)
        {
            nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::charIntro));
        }
        else
        {
            nChar.setTexture(TextureHolder::getInstance().get(Textures::char2Sprite));
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::charIntro2));
        }
    });
    nextButton.setPosition({792.5, 482.5});
    nextButton.setIsSelected(false);
    nextButton.setCallback([this, charSlot1] ()
    {
        nGUIContainer.selectPrevious();
        if(nGUIContainer.getSelectedChild() == charSlot1)
        {
            nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::charIntro));
        }
        else
        {
            nChar.setTexture(TextureHolder::getInstance().get(Textures::char2Sprite));
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::charIntro2));
        }
    });

    backButton.setPosition({75, 705});  
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChoosePlayer);
    }); 

    saveButton.setPosition({1102, 705});
    saveButton.setIsSelected(false);
    saveButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseMode);
    });

    instructionButton.setPosition({75, 92});
    instructionButton.setIsSelected(false);
    instructionButton.setCallback([this] ()
    {
        requestStackPush(States::Instruction);
    });

    chooseModeButton.setPosition({75, 221});
    chooseModeButton.setIsSelected(false);
    chooseModeButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseMode);
    });

    nGUIContainer.pack(charSlot1, true);
    nGUIContainer.pack(charSlot2, true);
}


void ChooseCharState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
    window.draw(previousButton);
    window.draw(nextButton);
    window.draw(nChar);
    window.draw(nCharIntro);
    window.draw(saveButton);
    window.draw(instructionButton);
    window.draw(chooseModeButton);
}

bool ChooseCharState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();

    if (saveButton.isMouseOver(window))
        saveButton.setSelectedSprite();
    else saveButton.setNormalSprite();

    if (instructionButton.isMouseOver(window))
        instructionButton.setSelectedSprite();
    else instructionButton.setNormalSprite();

    if (chooseModeButton.isMouseOver(window))
        chooseModeButton.setSelectedSprite();
    else chooseModeButton.setNormalSprite();

    return true;
}

bool ChooseCharState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        requestStackPop();
        requestStackPush(States::ChooseMode);
        return false;
    }
    nGUIContainer.handleEvent(event);
    backButton.handleEvent(event);
    previousButton.handleEvent(event);
    nextButton.handleEvent(event);
    saveButton.handleEvent(event);
    instructionButton.handleEvent(event);
    chooseModeButton.handleEvent(event);
    return false;
}
