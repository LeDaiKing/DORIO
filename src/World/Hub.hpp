#include<SFML/Graphics/RenderWindow.hpp>
#include "Dough.hpp"
#include "Animation.hpp"
class Hub
{
    public:
        Hub();
        void draw(sf::RenderWindow& window);
        void update(sf::Time dt, Dough& player, sf::Time& gameTime);
    private:
        Animation nClock;
        Animation nCoin;
        Animation nHeart;
        Animation nHeart2;
        // sf::Sprite nSettingBut;
        int preHitPoints;
        int curHitPoints;
        int curCoins;
        sf::Time nTime;
        // Dough* nPlayer;
        // sf::Vector2f
};

