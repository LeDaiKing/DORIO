#include "Enemy.hpp"

class EnemyFactory
{
    public:
        static std::unique_ptr<Enemy> createEnemy(Enemy::Type type, sf::Vector2f position);
    private:
        EnemyFactory() = default;
        ~EnemyFactory() = default;
        EnemyFactory(const EnemyFactory&) = delete;
        EnemyFactory& operator=(const EnemyFactory&) = delete;
};