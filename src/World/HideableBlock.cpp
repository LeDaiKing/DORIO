// #include "HideableBlock.hpp"
// #include "../Command/CommandQueue.hpp"
// #include "../Utility.hpp"
// #include <SFML/Graphics/RenderTarget.hpp>
// #include <SFML/Graphics/RenderStates.hpp>

// HideableBlock::HideableBlock(Type type, sf::Vector2f position)
//     : Block(type, position), characterHiding(false)
// {
//     // Initialize the block as needed
// }

// void HideableBlock::hideCharacter()
// {
//     characterHiding = true;
// }

// void HideableBlock::showCharacter()
// {
//     characterHiding = false;
// }

// bool HideableBlock::isCharacterHiding() const
// {
//     return characterHiding;
// }

// void HideableBlock::updateCurrent(sf::Time dt, CommandQueue& commands)
// {
//     // Update logic for the block, if any
//     Block::updateCurrent(dt, commands);
// }

// void HideableBlock::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     if (!characterHiding)
//     {
//         Block::drawCurrent(target, states);
//     }
// }