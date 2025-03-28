#include "ChoosePlayerState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include "../UI/Component.hpp"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

ChoosePlayerState::ChoosePlayerState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, handSprite()
, choosePlayerDeco()
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, settingButton(context, Textures::ID::SettingButtonNormal, Textures::ID::SettingButtonSelected, Textures::ID::SettingButtonPressed)
// , onePlayerButton(context, ButtonTextures::ID::onePlayerButton)
// , twoPlayerButton(context, ButtonTextures::ID::twoPlayerButton)
, drawHand(false)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));
    handSprite.setTexture(TextureHolder::getInstance().get(Textures::HandSprite));
    choosePlayerDeco.setPosition({189, 0});
    saveCurrentState();

    auto onePlayerButton = std::make_shared<GUI::Button>(context, Textures::ID::OnePlayerButtonNormal, Textures::ID::OnePlayerButtonSelected, Textures::ID::OnePlayerButtonPressed);
    onePlayerButton->setPosition({418, 437});
    onePlayerButton->setIsSelected(true);
    onePlayerButton->setCallback([this] ()
    {
        std::ofstream file(*getContext().saveFile + "numberPlayer.bin", std::ios::binary);
        int numberPlayer = 1;
        file.write((char*)&numberPlayer, sizeof(int));
        file.close();
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
        requestStackPush(States::ID::Transition);
    });

    auto twoPlayerButton = std::make_shared<GUI::Button>(context, Textures::ID::TwoPlayerButtonNormal, Textures::ID::TwoPlayerButtonSelected, Textures::ID::TwoPlayerButtonPressed);
    twoPlayerButton->setPosition({872, 443});
    twoPlayerButton->setIsSelected(true);
    twoPlayerButton->setCallback([this] ()
    {
        std::ofstream file(*getContext().saveFile + "numberPlayer.bin", std::ios::binary);
        int numberPlayer = 2;
        file.write((char*)&numberPlayer, sizeof(int));
        file.close();
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
        requestStackPush(States::ID::Transition);
    });
    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseMode);
        requestStackPush(States::ID::Transition);
    });  

    settingButton.setPosition({75, 92});
    settingButton.setIsSelected(false);
    settingButton.setCallback([this] ()
    {
        requestStackPush(States::Instruction);
        requestStackPush(States::ID::Transition);
    });

    nGUIContainer.pack(onePlayerButton);
    nGUIContainer.pack(twoPlayerButton);
}

ChoosePlayerState::~ChoosePlayerState() {
    
}

void ChoosePlayerState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(choosePlayerDeco);
    window.draw(backButton);
    window.draw(settingButton);
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
    if (settingButton.isMouseOver(window))
        settingButton.setSelectedSprite();
    else settingButton.setNormalSprite();
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
    settingButton.handleEvent(event);
    nGUIContainer.handleEvent(event);
    // onePlayerButton.handleEvent(event);
    // twoPlayerButton.handleEvent(event);
    return false;
}

void ChoosePlayerState::saveCurrentState() {
    Context context = getContext();
    std::ofstream savefile(*context.saveFile + "state.bin", std::ios::binary);
    assert(savefile.is_open());
    int state = States::ID::ChoosePlayer;
    savefile.write((char*)&state, sizeof(int));
    savefile.close();
}
