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
    saveCurrentState();

    auto kitchenMode = std::make_shared<GUI::Button>(context, Textures::ID::KitchenModeNormal, Textures::ID::KitchenModeSelected, Textures::ID::KitchenModePressed);
    kitchenMode->setPosition({407, 347});
    kitchenMode->setCallback([this] ()
    {   
        std::ofstream file(*getContext().saveFile + "level.bin", std::ios::binary);
        int level = 1;
        file.write((char*)&level, sizeof(int));
        file.close();
        requestStackPop();
        requestStackPush(States::ChoosePlayer);
        requestStackPush(States::Transition);
    });

    auto hallwayMode = std::make_shared<GUI::Button>(context, Textures::ID::HallwayModeNormal, Textures::ID::HallwayModeSelected, Textures::ID::HallwayModePressed);
    std::cout << "Hallway Mode create" << std::endl;
    hallwayMode->setPosition({655.565, 165.5});
    hallwayMode->setCallback([this] ()
    {
        std::ofstream file(*getContext().saveFile + "level.bin", std::ios::binary);
        int level = 2;
        file.write((char*)&level, sizeof(int));
        file.close();
        requestStackPop();
        requestStackPush(States::ChoosePlayer);
        requestStackPush(States::Transition);
    });

    auto gardenMode = std::make_shared<GUI::Button>(context, Textures::ID::GardenModeNormal, Textures::ID::GardenModeSelected, Textures::ID::GardenModePressed);
    gardenMode->setPosition({912.77, 345});
    gardenMode->setCallback([this] ()
    {
        std::ofstream file(*getContext().saveFile + "level.bin", std::ios::binary);
        int level = 3;
        file.write((char*)&level, sizeof(int));
        file.close();
        requestStackPop();
        requestStackPush(States::ChoosePlayer);
        requestStackPush(States::Loading);
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
        requestStackPush(States::ChooseSlot);
        requestStackPush(States::ID::Transition);
        getPreviousFolder(*getContext().saveFile);
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
        requestStackPush(States::ID::Transition);
    });
    chooseCharButton.setPosition({75, 221});
    chooseCharButton.setIsSelected(false);
    chooseCharButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
        requestStackPush(States::ID::Transition);
    });

    std::ifstream file(*context.saveFile + "map.bin", std::ios::binary);
    for (int i = 0; i < 3; i++) {
        bool isWin = false;
        bool isCoin = false;
        file.read((char*)&isWin, sizeof(bool));
        file.read((char*)&isCoin, sizeof(bool));
        std::cout << isWin << " " << isCoin << std::endl;
        if (isWin == false) {
            if (i == 0) {
                std::cout << "Hallway Mode Locked" << std::endl;
                hallwayMode->setNormalTexture(Textures::ID::HallwayModeLocked);
                hallwayMode->setIsSelected(false);
                hallwayMode->setIsPressable(false);
            }
            else if (i == 1) {
                gardenMode->setNormalTexture(Textures::ID::GardenModeLocked);
                gardenMode->setIsSelected(false);
                gardenMode->setIsPressable(false);
            }
        }

        if (isWin == true) {
            if (i == 0) kitchenModeBadge[0].setTexture(TextureHolder::getInstance().get(Textures::ThreeStarBadgeNormal));
            if (i == 1) hallwayModeBadge[0].setTexture(TextureHolder::getInstance().get(Textures::ThreeStarBadgeNormal));
            if (i == 2) gardenModeBadge[0].setTexture(TextureHolder::getInstance().get(Textures::ThreeStarBadgeNormal));
        }
        if (isCoin == true) {
            if (i == 0) kitchenModeBadge[1].setTexture(TextureHolder::getInstance().get(Textures::OneStarBadgeNormal));
            if (i == 1) hallwayModeBadge[1].setTexture(TextureHolder::getInstance().get(Textures::OneStarBadgeNormal));
            if (i == 2) gardenModeBadge[1].setTexture(TextureHolder::getInstance().get(Textures::OneStarBadgeNormal));
        }
    }

    nGUIContainer.pack(kitchenMode);
    nGUIContainer.pack(hallwayMode);
    nGUIContainer.pack(gardenMode);
    sf::RenderWindow& window = *getContext().window;    
    window.setKeyRepeatEnabled(true);    
}

ChooseModeState::~ChooseModeState() {
    std::cout << "ChooseModeState Destructor" << std::endl;
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

void ChooseModeState::saveCurrentState() {
    Context context = getContext();
    std::ofstream savefile(*context.saveFile + "state.bin", std::ios::binary);
    assert(savefile.is_open());
    int state = States::ID::ChooseMode;
    savefile.write((char*)&state, sizeof(int));
    savefile.close();
}
