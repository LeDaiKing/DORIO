#include "Hub.hpp"
#include "../Holder/TextRender.hpp"
#include "../Holder/ResourceHolder.hpp"
#include <string>

Hub::Hub()
: nHeart(TextureHolder::getInstance().get(Textures::HeartIcon))
, nHeart2(TextureHolder::getInstance().get(Textures::HeartIcon))
, nCoin(TextureHolder::getInstance().get(Textures::CoinIcon))
, nClock(TextureHolder::getInstance().get(Textures::ClockIcon))
// , nSettingBut(TextureHolder::getInstance().get(Textures::SettingBut))
, preHitPoints(-1)
, nTime(sf::Time::Zero)
, curHitPoints(0)
, curCoins(0)
, curScore(0)
{
    nCoin.addAnimationState(0, 0, 6, sf::seconds(0.5f), sf::Vector2i(16, 16), true);
    nCoin.setAnimationState(0);
    nClock.addAnimationState(0, 0, 5, sf::seconds(0.5f), sf::Vector2i(512, 512), true);
    nClock.setAnimationState(0);
    nHeart.addAnimationState(0, 0, 1, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
    nHeart.setAnimationState(0);
    nHeart2.addAnimationState(0, 0, 6, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
    nHeart2.update(sf::seconds(1.f));

    nCoin.setScale(50.f  / nCoin.getGlobalBounds().width, 50.f / nCoin.getGlobalBounds().height);
    nHeart.setScale(65.f / nHeart.getGlobalBounds().width, 65.f / nHeart.getGlobalBounds().height);
    nHeart2.setScale(65.f / nHeart2.getGlobalBounds().width, 65.f / nHeart2.getGlobalBounds().height);
    nClock.setScale(58.f / nClock.getGlobalBounds().width, 50.f / nClock.getGlobalBounds().height);
    // nSettingBut.setScale(66.f / nSettingBut.getGlobalBounds().width, 70.f / nSettingBut.getGlobalBounds().height);

    nCoin.setPosition(37 + nCoin.getGlobalBounds().width / 2, 92 + nCoin.getGlobalBounds().height / 2);
    nClock.setPosition(1039 + nClock.getGlobalBounds().width / 2, 29 + nClock.getGlobalBounds().height / 2);
    nHeart.setPosition(29 + nHeart.getGlobalBounds().width / 2, 19 + nHeart.getGlobalBounds().height / 2);

    // centerOrigin(nSettingBut);
    // nSettingBut.setPosition(1115 + nSettingBut.getGlobalBounds().width / 2, 29 + nSettingBut.getGlobalBounds().height / 2);
  
}

void Hub::draw(sf::RenderWindow& window)
{
    window.draw(nCoin);
    window.draw(nClock);
    // window.draw(nSettingBut);


    int size = 30;
    std::string coinText = std::to_string(curCoins);
    sf::Vector2f pos(85, 102);
    TextRender::getInstance().drawText(window, coinText.c_str(), pos, size);

    int time = nTime.asSeconds();
    std::string timeText = std::to_string(time);
    pos = {1089, 39};
    TextRender::getInstance().drawText(window, timeText.c_str(), pos, size);

    nHeart.setPosition(146 + nHeart.getGlobalBounds().width / 2, 29 + nHeart.getGlobalBounds().height / 2);
    
    if (!nHeart2.isFinished())
    {
        nHeart2.setPosition(29 + nHeart.getGlobalBounds().width / 2, 19 + nHeart.getGlobalBounds().height / 2);
        window.draw(nHeart2);
    }
    else 
    {
        nHeart.setPosition(29 + nHeart.getGlobalBounds().width / 2, 19 + nHeart.getGlobalBounds().height / 2);
        window.draw(nHeart);
    }
    std::string heartText = "x" + std::to_string(curHitPoints);
    pos = {85, 39};
    TextRender::getInstance().drawText(window, heartText.c_str(), pos, 30);


    size = 32;
    std::string scoreText = std::to_string(curScore);
    pos = {960, 29};
    float len = scoreText.size();
    len = len * 8 - (len - 1) * 2;
    len *= size / 8.f;
    pos.x -= len;
    TextRender::getInstance().drawText(window, scoreText.c_str(), pos, size);

}

void Hub::update(sf::Time dt, Dough& player, sf::Time& gameTime)
{
    curHitPoints = player.getHitPoints();
    curCoins = player.getCoinsCount();
    if (preHitPoints > curHitPoints)
    {
        nHeart2.setAnimationState(0);
    }
    preHitPoints = curHitPoints;
    curScore = player.getScore();
    nHeart2.update(dt);
    nCoin.update(dt);
    nClock.update(dt);

    nTime = gameTime;
}
