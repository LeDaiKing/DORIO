#include "ChooseCharState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <iostream>

ChooseCharState::ChooseCharState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, previousButton(context, Textures::ID::PreviousButtonNormal, Textures::ID::PreviousButtonSelected, Textures::ID::PreviousButtonPressed)
, nextButton(context, Textures::ID::NextButtonNormal, Textures::ID::NextButtonSelected, Textures::ID::NextButtonPressed)
, saveButton(context, Textures::ID::SaveButtonNormal, Textures::ID::SaveButtonSelected, Textures::ID::SaveButtonPressed)
, instructionButton(context, Textures::ID::InstructionButtonNormal, Textures::ID::InstructionButtonSelected, Textures::ID::InstructionButtonPressed)
, chooseModeButton(context, Textures::ID::ChooseModeButtonNormal, Textures::ID::ChooseModeButtonSelected, Textures::ID::ChooseModeButtonPressed)
, nCharAni1(TextureHolder::getInstance().get(Textures::Dough1))
, nCharAni2(TextureHolder::getInstance().get(Textures::Dough1))
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseCharScreen));
    nChar.setTexture(TextureHolder::getInstance().get(Textures::Char1Sprite));
    nChar.setPosition({376, 289});
    nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::CharIntro));
    nCharIntro.setPosition({208, 53});

    nCharAni1.addAnimationState(0, 96, 12, sf::seconds(1), sf::Vector2i(32, 32), true);
    nCharAni1.setAnimationState(0);
    nCharAni1.setPosition(538.5, 464);
    nCharAni1.setScale(5, 5);

    nCharAni2.addAnimationState(0, 96, 12, sf::seconds(1), sf::Vector2i(32, 32), true);
    nCharAni2.setAnimationState(0);
    nCharAni2.setPosition(538.5, 464);
    nCharAni2.setScale(5, 5);


    auto charSlot1 = std::make_shared<GUI::Button>(context, Textures::ID::CharSlot1Normal, Textures::ID::CharSlot1Selected, Textures::ID::CharSlot1Pressed);
    charSlot1->setPosition({930, 131});
    charSlot1->setCallback([this] ()
    {
        // nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
        selectedChar = 1;
    });
    auto charSlot2 = std::make_shared<GUI::Button>(context, Textures::ID::CharSlot2Normal, Textures::ID::CharSlot2Selected, Textures::ID::CharSlot2Pressed);
    charSlot2->setPosition({1088, 131});
    charSlot2->setCallback([this] ()
    {
        // nChar.setTexture(TextureHolder::getInstance().get(Textures::char2Sprite));
        selectedChar = 2;
    });
    
    previousButton.setPosition({284.5, 482.5});
    previousButton.setIsSelected(false);
    previousButton.setCallback([this, charSlot1] ()
    {
        nGUIContainer.selectNext();
        if(nGUIContainer.getSelectedChild() == charSlot1)
        {
            // nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
            selectedChar = 1;
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::CharIntro));
        }
        else
        {
            // nChar.setTexture(TextureHolder::getInstance().get(Textures::char2Sprite));
            selectedChar = 2;
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::CharIntro2));
        }
    });
    nextButton.setPosition({792.5, 482.5});
    nextButton.setIsSelected(false);
    nextButton.setCallback([this, charSlot1] ()
    {
        nGUIContainer.selectPrevious();
        if(nGUIContainer.getSelectedChild() == charSlot1)
        {
            // nChar.setTexture(TextureHolder::getInstance().get(Textures::char1Sprite));
            selectedChar = 1;
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::CharIntro));
        }
        else
        {
            // nChar.setTexture(TextureHolder::getInstance().get(Textures::char2Sprite));
            selectedChar = 2;
            nCharIntro.setTexture(TextureHolder::getInstance().get(Textures::CharIntro2));
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
    if (selectedChar == 1)  {
        window.draw(nCharAni1);
    }
    else {
        window.draw(nCharAni2);
    }

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

    nCharAni1.update(dt);
    nCharAni2.update(dt);

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
