// #include "LuckyBlock.hpp"
// #include  "../Holder/ResourceIdentifiers.hpp"
// #include "../Holder/ResourceHolder.hpp"
// #include <SFML/Graphics/RenderTarget.hpp>
// #include <SFML/Graphics/RenderStates.hpp>
// #include "Utility.hpp"
// #include "Entity.hpp"
// #include "World.hpp"


// LuckyBlock::LuckyBlock(Type type, sf::Vector2f position)
// : BouncingBlock(type, position)
// {
//     nIsEmpty = true;
// }

// void LuckyBlock::applyNormal(SceneNode& graph)
// {
//     if (nIsEmpty)
//         StaticBlock::applyNormal(graph);
//     else
//         BouncingBlock::applyNormal(graph);
// }

// sf::FloatRect LuckyBlock::getBoundingRect() const
// {
//     if (nIsEmpty)
//         return StaticBlock::getBoundingRect();
//     else
//         return BouncingBlock::getBoundingRect();
// }

// void LuckyBlock::updateCurrent(sf::Time dt)
// {
//     if (!nIsEmpty)
//         BouncingBlock::updateCurrent(dt);
// }

// void LuckyBlock::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     if (nIsEmpty)
//         StaticBlock::drawCurrent(target, states);
//     else
//         BouncingBlock::drawCurrent(target, states);
// }