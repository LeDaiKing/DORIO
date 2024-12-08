// #include "Projectile.hpp"
// #include "Entity.hpp"
// #include "Utility.hpp"


// Projectile::Projectile(Type type, sf::Vector2f position)
// : Item(type, position)
// , nSpeed(200.f)
// , nDamage(1)z
// , nTargetCategory(Category::PlayerDough)
// {
//     nAnimation.addAnimationState(0, 0, 1, sf::seconds(0.5f), sf::Vector2i(25, 25), true);
//     nAnimation.setAnimationState(0);
// }
// void Projectile::activate(Entity& player)
// {
//     nIsCollected = true;
// }

// void Projectile::setSpeed(float speed)
// {
//     nSpeed = speed;
// }

// void Projectile::setDamage(unsigned int damage)
// {
//     nDamage = damage;
// }

// void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
// {
//     Item::updateCurrent(dt, commands);
//     if (!nIsCollected)
//     {
//         // sf::Vector2f velocity = getVelocity();
//         move(nVelocity * dt.asSeconds());
//     }
// }

// void Projectile::setTargetCategory(unsigned int category)
// {
//     nTargetCategory = category;
// }

// void Projectile::setTargetDirection(sf::Vector2f direction)
// {
//     nVelocity = direction * nSpeed;
// }

// void Projectile::setTargetPosition(sf::Vector2f position)
// {
//     sf::Vector2f direction = position - getPosition();
//     direction /= length(direction);
//     setTargetDirection(direction);
// }

// void Projectile::destroy()
// {
//     nIsCollected = true;
// }