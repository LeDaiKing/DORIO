#include "WinningState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>
#include <iostream>


WinningState::WinningState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nCharacterSprite()
, leaderboardButton(context, Textures::ID::LeaderboardButtonNormal, Textures::ID::LeaderboardButtonSelected, Textures::ID::LeaderboardButtonPressed)
, startButton(context, Textures::ID::StartButtonNormal, Textures::ID::StartButtonSelected, Textures::ID::StartButtonPressed)
, nClock()
, nHeart()
, textClock("")
, textHeart("")
, textScore("")
, winBadge()
, coinBadge()
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::LeaderboardScreen));
    
    nCharacterSprite.setTexture(TextureHolder::getInstance().get(Textures::Char1Sprite));
    nCharacterSprite.setPosition(792, 238);

    nClock.setTexture(TextureHolder::getInstance().get(Textures::ClockHub));
    nClock.setPosition(169, 370);

    nHeart.setTexture(TextureHolder::getInstance().get(Textures::HeartHub));
    nHeart.setPosition(169, 370);

    textClock.setPosition(446.5, 432);
    textClock.setColor(sf::Color::Black);
    textClock.setCharacterSize(45);

    textHeart.setPosition(446.5, 432);
    textHeart.setColor(sf::Color::Black);
    textHeart.setCharacterSize(45);

    textScore.setPosition(373, 267);
    textScore.setColor(sf::Color::Black);
    textScore.setCharacterSize(60);
    
    leaderboardButton.setPosition(955, 711);
    leaderboardButton.setIsSelected(false);
    leaderboardButton.setCallback([this] ()
    {
        // requestStackPush(States::Leaderboard);
    });

    startButton.setPosition(1103, 711);
    startButton.setIsSelected(false);
    startButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseMode);
    });

    std::ifstream file("file/Score/score.txt");
    file >> nSecond >> nHP >> nScore;
    bool isAchieveWin, isAchieveCoin;
    file >> isAchieveWin >> isAchieveCoin;
    if (isAchieveWin) winBadge.setTexture(TextureHolder::getInstance().get(Textures::ThreeStarBadgeNormal));
    else winBadge.setTexture(TextureHolder::getInstance().get(Textures::ThreeStarBadgeBlank));
    if (isAchieveCoin) coinBadge.setTexture(TextureHolder::getInstance().get(Textures::OneStarBadgeNormal));
    else coinBadge.setTexture(TextureHolder::getInstance().get(Textures::OneStarBadgeBlank));
    winBadge.setPosition(420, 400);
    coinBadge.setPosition(231, 402);
    winBadge.setScale(1.85, 1.85);
    coinBadge.setScale(1.85, 1.85);
}


void WinningState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(nBackgroundSprite);
    window.draw(nCharacterSprite);
    window.draw(leaderboardButton);
    window.draw(startButton);
    textScore.setText(std::to_string((int) nScore));
    window.draw(textScore);
    

    if (isEnd == false) {
        if (nSecond > 0 || delaySecond > 0) {
            window.draw(nClock);
            textClock.setText(std::to_string((int) nSecond));
            window.draw(textClock);
        }
        else if (nHP > 0 || delayHP > 0) {
            window.draw(nHeart);
            textHeart.setText(std::to_string((int) nHP));
            window.draw(textHeart);
        }
    }
    else {
        winBadge.setColor(sf::Color(255, 255, 255, alphaWin));
        coinBadge.setColor(sf::Color(255, 255, 255, alphaCoin));
        window.draw(winBadge);
        window.draw(coinBadge);
    }
}

bool WinningState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    if (delaySecond > 0) delaySecond -= dt.asSeconds();
    if (delayHP > 0) delayHP -= dt.asSeconds();

    if (isEnd == false) {
        if (delayUpdate > 0) {
            delayUpdate -= dt.asSeconds();
        }
        else if (nSecond > 0.0f) {
            nSecond -= dt.asSeconds() * speed;
            nScore += std::min(nSecond, dt.asSeconds() * speed) * secondToScore;
            if (nSecond < 0.0f) {
                nSecond = 0.0f;
                delaySecond = DELAY;
                delayUpdate = DELAY * 2;
            }
        }
        else {
            if (nHP > 0) {
                nHP -= (dt.asSeconds() * speed);
                nScore += std::min(nHP, dt.asSeconds() * speed) * hpToScore;
                if (nHP < 0) {
                    nHP = 0;
                    delayHP = DELAY;
                    delayUpdate = DELAY;
                }
            }
            else {
                isEnd = true;
            }
        }
    }
    else {
        alphaCoin += dt.asSeconds() * speed;
        alphaWin += dt.asSeconds() * speed;
        if (alphaCoin > 255) alphaCoin = 255;
        if (alphaWin > 255) alphaWin = 255;
    }
    return true;
}

bool WinningState::handleEvent(const sf::Event& event)
{
    leaderboardButton.handleEvent(event);
    startButton.handleEvent(event);
    return false;
}
