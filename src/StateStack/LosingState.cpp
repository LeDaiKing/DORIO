#include "LosingState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"

LosingState::LosingState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, leaderboardButton(context, Textures::ID::LeaderboardButtonNormal, Textures::ID::LeaderboardButtonSelected, Textures::ID::LeaderboardButtonPressed)
, homeButton(context, Textures::ID::HomeButtonNormal, Textures::ID::HomeButtonSelected, Textures::ID::HomeButtonPressed)
, retryButton(context, Textures::ID::RetryButtonNormal, Textures::ID::RetryButtonSelected, Textures::ID::RetryButtonPressed)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::LosingScreen));
    
    leaderboardButton.setPosition(955, 700);
    leaderboardButton.setIsSelected(false);
    leaderboardButton.setCallback([this] ()
    {
        // requestStackPop();
        // requestStackPush(States::Leaderboard);
    });

    homeButton.setPosition(1096, 700);
    homeButton.setIsSelected(false);
    homeButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    retryButton.setPosition(591, 637);
    retryButton.setIsSelected(false);
    retryButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });
}

void LosingState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(leaderboardButton);
    window.draw(homeButton);
    window.draw(retryButton);
}

bool LosingState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (leaderboardButton.isMouseOver(window))
        leaderboardButton.setSelectedSprite();
    else leaderboardButton.setNormalSprite();

    if (homeButton.isMouseOver(window))
        homeButton.setSelectedSprite();
    else homeButton.setNormalSprite();

    if (retryButton.isMouseOver(window))
        retryButton.setSelectedSprite();
    else retryButton.setNormalSprite();

    return true;
}

bool LosingState::handleEvent(const sf::Event& event)
{
    leaderboardButton.handleEvent(event);
    homeButton.handleEvent(event);
    retryButton.handleEvent(event);
    return false;
}