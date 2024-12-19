#include "EnemyFactory.hpp"
#include "CockRoach.hpp"
#include "Ghost.hpp"
#include "Chicken.hpp"
#include "Snail.hpp"
#include "SnailShell.hpp"


std::unique_ptr<Enemy> EnemyFactory::createEnemy(Enemy::Type type, sf::Vector2f position)
{
    switch (type)
    {
    case Enemy::Type::CockRoach:
        return std::make_unique<CockRoach>(type, position);
    case Enemy::Type::Ghost:
        return std::make_unique<Ghost>(type, position);
    case Enemy::Type::Chicken:
        return std::make_unique<Chicken>(type, position);
    case Enemy::Type::Snail:
        return std::make_unique<Snail>(type, position);
    case Enemy::Type::SnailShell:
        return std::make_unique<SnailShell>(type, position);
    default:
        return nullptr;
    }
}