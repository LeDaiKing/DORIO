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
{
    rectButton.setPosition(600.5, 400);
    rectButton.setIsSelected(false);

    auto homeButton = std::make_shared<GUI::Button>(context, Textures::ID::HomeButtonNormal, Textures::ID::HomeButtonSelected, Textures::ID::HomeButtonPressed);
    homeButton->setPosition(386, 400);
    homeButton->setCallback([this] ()
    {
        requestStackClear();
        requestStackPush(States::Title);
    });

    auto saveButton = std::make_shared<GUI::Button>(context, Textures::ID::SaveButtonNormal, Textures::ID::SaveButtonSelected, Textures::ID::SaveButtonPressed);
    saveButton->setPosition(600, 400);
    saveButton->setCallback([this] ()
    {
        // save game
        std::ifstream file("file/CurSave/save.txt");
        std::string saveFile;
        file >> saveFile;
        file.close();
        std::ofstream savefile(saveFile, std::ios::binary);
        std::ifstream temp("file/CurSave/temp", std::ios::binary);
        savefile << temp.rdbuf();
        temp.close();
        savefile.close();
    });

    auto startButton = std::make_shared<GUI::Button>(context, Textures::ID::StartButtonNormal, Textures::ID::StartButtonSelected, Textures::ID::StartButtonPressed);
    startButton->setPosition(814, 400); 
    startButton->setCallback([this] ()
    {
        requestStackPop();
    });


    nBar.pack(homeButton);
    nBar.pack(saveButton);
    nBar.pack(startButton);
    nBar.selectNext();
    nBar.selectNext();
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
}

bool PauseState::update(sf::Time dt) {
    return false;
}

bool PauseState::handleEvent(const sf::Event& event) {
    nBar.handleEvent(event);
    return false;
}

