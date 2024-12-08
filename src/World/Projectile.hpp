// #pragma once
// #include "Item.hpp"
// class Entity;

// class Projectile : public Item
// {
//     public:
//         Projectile(Type type, sf::Vector2f position);
//         virtual void activate(Entity& player) override;
//         void setSpeed(float speed);
//         void setDamage(unsigned int damage);
//         void setTargetCategory(unsigned int category);
//         void setTargetDirection(sf::Vector2f direction);
//         void setTargetPosition(sf::Vector2f position);
//         void destroy();
//     protected:
//         virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
//     private:
//         sf::Vector2f nVelocity;
//         float nSpeed;
//         unsigned int nDamage;
//         unsigned int nTargetCategory;
// };