#include "StaticBlock.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Utility.hpp"

StaticBlock::StaticBlock(Type type, sf::Vector2f position)
: Block(type, position)
{
}

// void StaticBlock::applyNormal(SceneNode& graph)
// {
//     if (graph.getCategory() & Category::Entity)
//     {
//         assert(dynamic_cast<Entity*>(&graph) != nullptr);
//         Entity& entity = static_cast<Entity&>(graph);

//         sf::FloatRect entityHitBox = entity.getBoundingRect();
//         sf::FloatRect bound = getBoundingRect();

//         collision::Side side = checkCollisionSide(entityHitBox, bound);
//         int gravity = World::getGravity();

//         if (side == collision::Top && entity.getVelocity().y > 0)
//         {
//             entity.setPosition(entity.getPosition().x, bound.top - entityHitBox.height / 2);
//             entity.setVelocity(entity.getVelocity().x, 0.f);
//             entity.accelerate(0.f, -gravity);
//             entity.setOnGround(true);
//         }
//         else if (side == collision::Bottom && entity.getVelocity().y < 0)
//         {
//             entity.setPosition(entity.getPosition().x, bound.top + bound.height + entityHitBox.height / 2);
//             entity.setVelocity(entity.getVelocity().x, 0.f);
//             // entity.accelerate(0.f, -gravity);

//         }
//         else if (side == collision::Left && entity.getVelocity().x > 0)
//         {
//             entity.setPosition(bound.left - entityHitBox.width / 2, entity.getPosition().y);
//             entity.setVelocity(0.f, entity.getVelocity().y);
//         }
//         else if (side == collision::Right && entity.getVelocity().x < 0)
//         {
//             entity.setPosition(bound.left + bound.width + entityHitBox.width / 2, entity.getPosition().y);
//             entity.setVelocity(0.f, entity.getVelocity().y);
//         }


//     }

//     for (Ptr& child : graph.getChildren())
//     {
//         applyNormal(*child);
//     }
// }

void StaticBlock::handleTopCollision(Entity& entity)
{
    entity.move(0.f, getBoundingRect().top - entity.getBoundingRect().top - entity.getBoundingRect().height);
    entity.accelerate(0.f, -World::getGravity());
    entity.setVelocity(entity.getVelocity().x, 0.f);
}

void StaticBlock::handleBottomCollision(Entity& player)
{
    sf::FloatRect bound = getBoundingRect();
    player.move(0.f, bound.top + bound.height - player.getBoundingRect().top);
    player.setVelocity(player.getVelocity().x, 0.f);
    // player.setOnGround(true);
}