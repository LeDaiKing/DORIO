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
{
    nCoin.addAnimationState(0, 0, 6, sf::seconds(0.5f), sf::Vector2i(16, 16), true);
    nCoin.setAnimationState(0);
    nClock.addAnimationState(0, 0, 5, sf::seconds(0.5f), sf::Vector2i(512, 512), true);
    nClock.setAnimationState(0);
    nHeart.addAnimationState(0, 0, 1, sf::seconds(0.5f), sf::Vector2i(32, 32), true);
    nHeart.setAnimationState(0);
    nHeart2.addAnimationState(0, 0, 6, sf::seconds(0.6f), sf::Vector2i(32, 32), false);
    nHeart2.setAnimationState(0);
    nHeart2.update(sf::seconds(1.f));

    nCoin.setScale(60.f  / nCoin.getGlobalBounds().width, 60.f / nCoin.getGlobalBounds().height);
    nHeart.setScale(50.f / nHeart.getGlobalBounds().width, 50.f / nHeart.getGlobalBounds().height);
    nHeart2.setScale(50.f / nHeart2.getGlobalBounds().width, 50.f / nHeart2.getGlobalBounds().height);
    nClock.setScale(68.f / nClock.getGlobalBounds().width, 60.f / nClock.getGlobalBounds().height);
    // nSettingBut.setScale(66.f / nSettingBut.getGlobalBounds().width, 70.f / nSettingBut.getGlobalBounds().height);

    nCoin.setPosition(445 + nCoin.getGlobalBounds().width / 2, 29 + nCoin.getGlobalBounds().height / 2);
    nClock.setPosition(772 + nClock.getGlobalBounds().width / 2, 29 + nClock.getGlobalBounds().height / 2);
    nHeart.setPosition(146 + nHeart.getGlobalBounds().width / 2, 29 + nHeart.getGlobalBounds().height / 2);

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
    sf::Vector2f pos(475, 90);
    float len = coinText.size() * 8;
    len -= (coinText.size() - 1) * 2;
    len *= size / 8.f;
    pos.x -= len / 2;
    TextRender::getInstance().drawText(window, coinText.c_str(), pos, size);

    int time = nTime.asSeconds();
    std::string minuteText = std::to_string(time / 60); if (minuteText.size() == 1) minuteText = "0" + minuteText;
    std::string secondText = std::to_string(time % 60); if (secondText.size() == 1) secondText = "0" + secondText;
    std::string timeText = minuteText + ":" + secondText;
    pos = {800, 90};
    len = timeText.size() * 8;
    len -= (timeText.size() - 1) * 2;
    len *= size / 8.f;
    pos.x -= len / 2;
    TextRender::getInstance().drawText(window, timeText.c_str(), pos, size);

    nHeart.setPosition(146 + nHeart.getGlobalBounds().width / 2, 29 + nHeart.getGlobalBounds().height / 2);
    for (int i = 0; i < curHitPoints; i++)
    {
        window.draw(nHeart);
        if (i >= 4 && i % 4 == 0)
        nHeart.move(0, 50);
        else
        nHeart.move(50, 0);
    }
    if (!nHeart2.isFinished())
    {
        nHeart2.setPosition(nHeart.getPosition());
        window.draw(nHeart2);
    }
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
    nHeart2.update(dt);
    nCoin.update(dt);
    nClock.update(dt);

    nTime = gameTime;
}
