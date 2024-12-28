#include "MusicState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


MusicState::MusicState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, instructionButton(context, Textures::ID::InstructionButtonNormal, Textures::ID::InstructionButtonSelected, Textures::ID::InstructionButtonPressed)
, musicButton(context, Textures::ID::MusicButtonNormal, Textures::ID::MusicButtonSelected, Textures::ID::MusicButtonPressed)
, nMusicBar(context)
, nSoundBar(context)
, nGUIContainerText()
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    instructionButton.setPosition({75, 92});
    instructionButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Instruction);
    });

    musicButton.setPosition({75, 221});
    musicButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Music);
    });

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
    });

    auto musicText = std::make_shared<GUI::Label>("Music");
    musicText->setPosition(600, 150);
    musicText->setColor(sf::Color::White);
    musicText->setCharacterSize(45);
    nGUIContainerText.pack(musicText);

    auto soundText = std::make_shared<GUI::Label>("Sound");
    soundText->setPosition(600, 400);
    soundText->setColor(sf::Color::White);
    soundText->setCharacterSize(45);
    nGUIContainerText.pack(soundText);


    nMusicBar.setPosition(316, 400);
    nSoundBar.setPosition(316, 564);
    nMusicBar.setVolume(context.music->getVolume());
    nSoundBar.setVolume(context.sounds->getVolume());
}

void MusicState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(nBackgroundSprite);
    window.draw(backButton);
    window.draw(nGUIContainerText);
    window.draw(instructionButton);
    window.draw(musicButton);
    window.draw(nMusicBar);
    window.draw(nSoundBar);
}

bool MusicState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (backButton.isMouseOver(window))
        backButton.setSelectedSprite();
    else backButton.setNormalSprite();

    if (instructionButton.isMouseOver(window))
        instructionButton.setSelectedSprite();
    else instructionButton.setNormalSprite();

    if (musicButton.isMouseOver(window))
        musicButton.setSelectedSprite();
    else musicButton.setNormalSprite();

    State::Context context = getContext();
    context.music->setVolume(nMusicBar.getVolume());
    context.sounds->setVolume(nSoundBar.getVolume());

    return true;
}

bool MusicState::handleEvent(const sf::Event& event)
{
    backButton.handleEvent(event);
    musicButton.handleEvent(event);
    instructionButton.handleEvent(event);
    nMusicBar.handleEvent(event);
    nSoundBar.handleEvent(event);
    return false;
}
