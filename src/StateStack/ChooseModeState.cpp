#include "ChooseModeState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"


ChooseModeState::ChooseModeState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, GUI::Button::Type::BackButton)
// , playStartButton(context, GUI::Button::Type::playStartButton)
, choosePlayerButton(context, GUI::Button::Type::choosePlayerButton)
, chooseCharButton(context, GUI::Button::Type::chooseCharButton)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    auto kitchenMode = std::make_shared<GUI::Button>(context, GUI::Button::Type::kitchenMode);
    kitchenMode->setPosition({407, 347});
    kitchenMode->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
        requestStackPush(States::Loading);
    });

    auto hallwayMode = std::make_shared<GUI::Button>(context, GUI::Button::Type::hallwayMode);
    hallwayMode->setPosition({655.565, 165.5});
    hallwayMode->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
        requestStackPush(States::Loading);
    });

    auto gardenMode = std::make_shared<GUI::Button>(context, GUI::Button::Type::gardenMode);
    gardenMode->setPosition({912.77, 345});
    gardenMode->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
        requestStackPush(States::Loading);
    });

    auto creativeMode = std::make_shared<GUI::Button>(context, GUI::Button::Type::creativeMode);
    creativeMode->setPosition({655.5, 555});

    kitchenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::oneStarBadgeBlank)));
    kitchenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::threeStarBadgeBlank)));
    kitchenModeBadge[0].setPosition({237, 256});
    kitchenModeBadge[1].setPosition({237, 335});

    hallwayModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::oneStarBadgeBlank)));
    hallwayModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::threeStarBadgeBlank)));
    hallwayModeBadge[0].setPosition({442, 60});
    hallwayModeBadge[1].setPosition({442, 139});


    gardenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::oneStarBadgeBlank)));
    gardenModeBadge.push_back(sf::Sprite(TextureHolder::getInstance().get(Textures::threeStarBadgeBlank)));
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

    choosePlayerButton.setPosition({75, 92});
    choosePlayerButton.setIsSelected(false);
    choosePlayerButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChoosePlayer);
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
}

void ChooseModeState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
    // window.draw(playStartButton);
    window.draw(choosePlayerButton);
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

    if (choosePlayerButton.isMouseOver(window))
        choosePlayerButton.setSelectedSprite();
    else choosePlayerButton.setNormalSprite();

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
    choosePlayerButton.handleEvent(event);
    chooseCharButton.handleEvent(event);
    return false;
}
