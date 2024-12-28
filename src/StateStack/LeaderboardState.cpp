#include "LeaderboardState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"

LeaderboardState::LeaderboardState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, homeButton(context, Textures::ID::HomeButtonNormal, Textures::ID::HomeButtonSelected, Textures::ID::HomeButtonPressed)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::LeaderboardScreen));
    homeButton.setPosition(1099, 713);
    homeButton.setIsSelected(false);
    homeButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });
}

void LeaderboardState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(homeButton);
}

bool LeaderboardState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (homeButton.isMouseOver(window))
        homeButton.setSelectedSprite();
    else homeButton.setNormalSprite();
    return true;
}   

bool LeaderboardState::handleEvent(const sf::Event& event)
{
    homeButton.handleEvent(event);
    return false;
}