#include "ChooseModeState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


ChooseModeState::ChooseModeState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
// , playStartButton(context, ButtonTextures::ID::playStartButton)
, settingButton(context, Textures::ID::SettingButtonNormal, Textures::ID::SettingButtonSelected, Textures::ID::SettingButtonPressed)
, chooseCharButton(context, Textures::ID::ChooseCharButtonNormal, Textures::ID::ChooseCharButtonSelected, Textures::ID::ChooseCharButtonPressed)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    auto kitchenMode = std::make_shared<GUI::Button>(context, Textures::ID::KitchenModeNormal, Textures::ID::KitchenModeSelected, Textures::ID::KitchenModePressed);
    kitchenMode->setPosition({407, 347});
    kitchenMode->setCallback([this] ()
    {   
        std::ofstream file("file/CurSave/save.txt", std::ios::app);
        file << "/level1";
        file.close();
        requestStackPop();
        requestStackPush(States::Game);
        requestStackPush(States::Loading);
    });

    auto hallwayMode = std::make_shared<GUI::Button>(context, Textures::ID::HallwayModeNormal, Textures::ID::HallwayModeSelected, Textures::ID::HallwayModePressed);
    hallwayMode->setPosition({655.565, 165.5});
    hallwayMode->setCallback([this] ()
    {
        std::ofstream file("file/CurSave/save.txt", std::ios::app);
        file << "/level2";
        file.close();
        requestStackPop();
        requestStackPush(States::Game);
        requestStackPush(States::Loading);
    });

    auto gardenMode = std::make_shared<GUI::Button>(context, Textures::ID::GardenModeNormal, Textures::ID::GardenModeSelected, Textures::ID::GardenModePressed);
    gardenMode->setPosition({912.77, 345});
    gardenMode->setCallback([this] ()
    {
        std::ofstream file("file/CurSave/save.txt", std::ios::app);
        file << "/level3";
        file.close();
        requestStackPop();
        requestStackPush(States::Game);
        requestStackPush(States::Loading);
    });

    auto creativeMode = std::make_shared<GUI::Button>(context, Textures::ID::CreativeModeNormal, Textures::ID::CreativeModeSelected, Textures::ID::CreativeModePressed);
    creativeMode->setPosition({655.5, 555});
    creativeMode->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Creative);
    });

    kitchenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::OneStarBadgeBlank)));
    kitchenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::ThreeStarBadgeBlank)));
    kitchenModeBadge[0].setPosition({237, 256});
    kitchenModeBadge[1].setPosition({237, 335});

    hallwayModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::OneStarBadgeBlank)));
    hallwayModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::ThreeStarBadgeBlank)));
    hallwayModeBadge[0].setPosition({442, 60});
    hallwayModeBadge[1].setPosition({442, 139});


    gardenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::OneStarBadgeBlank)));
    gardenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::ThreeStarBadgeBlank)));
    gardenModeBadge[0].setPosition({755, 256});
    gardenModeBadge[1].setPosition({755, 335});

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    // playStartButton.setPosition({1077, 681});
    // playStartButton.setIsSelected(false);
    // playStartButton.setCallback([this] ()
    // {
    //     requestStackPop();
    //     requestStackPush(States::Game);
    //     requestStackPush(States::Loading);
    // });

    settingButton.setPosition({75, 92});
    settingButton.setIsSelected(false);
    settingButton.setCallback([this] ()
    {
        requestStackPush(States::Instruction);
    });
    chooseCharButton.setPosition({75, 221});
    chooseCharButton.setIsSelected(false);
    chooseCharButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    nGUIContainer.pack(kitchenMode);
    nGUIContainer.pack(hallwayMode);
    nGUIContainer.pack(gardenMode);
    nGUIContainer.pack(creativeMode);
    sf::RenderWindow& window = *getContext().window;    
    window.setKeyRepeatEnabled(true);    
}

void ChooseModeState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
    // window.draw(playStartButton);
    window.draw(settingButton);
    window.draw(chooseCharButton);
    for (auto& badge : kitchenModeBadge)
        window.draw(badge);
    for (auto& badge : hallwayModeBadge)
        window.draw(badge);
    for (auto& badge : gardenModeBadge)
        window.draw(badge);
}

bool ChooseModeState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();

    // if (playStartButton.isMouseOver(window))
    //     playStartButton.setSelectedSprite();
    // else playStartButton.setNormalSprite();

    if (settingButton.isMouseOver(window))
        settingButton.setSelectedSprite();
    else settingButton.setNormalSprite();

    if (chooseCharButton.isMouseOver(window))
        chooseCharButton.setSelectedSprite();
    else chooseCharButton.setNormalSprite();
    return true;
}

bool ChooseModeState::handleEvent(const sf::Event& event)
{
    nGUIContainer.handleEvent(event);
    backButton.handleEvent(event);
    // playStartButton.handleEvent(event);
    settingButton.handleEvent(event);
    chooseCharButton.handleEvent(event);
    return false;
}
