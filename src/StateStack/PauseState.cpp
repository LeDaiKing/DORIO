#include "PauseState.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include "State.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../UI/Button.hpp"
#include <string>   
#include <fstream>

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, rectButton(context, Textures::ID::RectButtonNormal, Textures::ID::RectButtonSelected, Textures::ID::RectButtonPressed)
, nBar()
, nMusicBar(context)
, nSoundBar(context)
{
    rectButton.setPosition(600, 419.5);
    rectButton.setIsSelected(false);

    auto homeButton = std::make_shared<GUI::Button>(context, Textures::ID::HomeButtonNormal, Textures::ID::HomeButtonSelected, Textures::ID::HomeButtonPressed);
    homeButton->setPosition(396, 256);
    homeButton->setCallback([this] ()
    {
        requestStackClear();
        requestStackPush(States::ChooseSlot);
        getPreviousFolder(*getContext().saveFile);
    });

    auto saveButton = std::make_shared<GUI::Button>(context, Textures::ID::SaveButtonNormal, Textures::ID::SaveButtonSelected, Textures::ID::SaveButtonPressed);
    saveButton->setPosition(600, 256);
    saveButton->setCallback([this] ()
    {
        // save game
        std::ofstream savefile(*getContext().saveFile + "data.bin", std::ios::binary);
        std::ifstream temp("file/CurSave/temp", std::ios::binary);
        savefile << temp.rdbuf();
        temp.close();
        savefile.close();
    });

    auto startButton = std::make_shared<GUI::Button>(context, Textures::ID::StartButtonNormal1, Textures::ID::StartButtonSelected, Textures::ID::StartButtonPressed);
    startButton->setPosition(804, 256); 
    startButton->setCallback([this] ()
    {
        requestStackPop();
    });


    nBar.pack(homeButton);
    nBar.pack(saveButton);
    nBar.pack(startButton);
    nBar.selectNext();
    nBar.selectNext();

    nMusicBar.setPosition(360, 400);
    nSoundBar.setPosition(360, 550);
    nMusicBar.setVolume(context.music->getVolume());
    nSoundBar.setVolume(context.sounds->getVolume());
}

void PauseState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(rectButton);
    window.draw(this->nBar);
    window.draw(nMusicBar);
    window.draw(nSoundBar);
}

bool PauseState::update(sf::Time dt) {
    State::Context context = getContext();
    context.music->setVolume(nMusicBar.getVolume());
    context.sounds->setVolume(nSoundBar.getVolume());
    return false;
}

bool PauseState::handleEvent(const sf::Event& event) {
    nBar.handleEvent(event);
    nMusicBar.handleEvent(event);
    nSoundBar.handleEvent(event);
    return false;
}

