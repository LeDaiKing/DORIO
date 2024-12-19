#include "BlockFactory.hpp"
#include "BreakableBlock.hpp"
#include "StaticBlock.hpp"
#include "JumpyBlock.hpp"
#include "SlideBlock.hpp"
#include "LuckyBlock.hpp"

std::unique_ptr<Block> BlockFactory::createBlock(const std::string& type, sf::Vector2f position)
{
    if (type == "Floor1")
        return std::make_unique<StaticBlock>(Block::Type::Floor1, position);
    else if (type == "UnderFloor1")
        return std::make_unique<StaticBlock>(Block::Type::UnderFloor1, position);
    else if (type == "StaticBlock1")
        return std::make_unique<StaticBlock>(Block::Type::StaticBlock1, position);
    else if (type == "Breakable1")
        return std::make_unique<BreakableBlock>(Block::Type::Breakable1, position);
    else if (type == "LuckyBlock1")
        return std::make_unique<LuckyBlock>(Block::Type::LuckyBlock1, position);
    else if (type == "Floor2")
        return std::make_unique<StaticBlock>(Block::Type::Floor2, position);
    else if (type == "UnderFloor2")
        return std::make_unique<StaticBlock>(Block::Type::UnderFloor2, position);
    else if (type == "StaticBlock2")
        return std::make_unique<StaticBlock>(Block::Type::StaticBlock2, position);
    else if (type == "Breakable2")
        return std::make_unique<BreakableBlock>(Block::Type::Breakable2, position);
    else if (type == "LuckyBlock2")
        return std::make_unique<LuckyBlock>(Block::Type::LuckyBlock2, position);
    else if (type == "SlideBlock")
        return std::make_unique<SlideBlock>(Block::Type::SlideBlock, position);
    else if (type == "JumpyBlock")
        return std::make_unique<JumpyBlock>(Block::Type::JumpyBlock, position);
    else
        return nullptr;
}

std::unique_ptr<Block> BlockFactory::createBlock(Block::Type type, sf::Vector2f position)
{
    switch (type)
    {
    case Block::Type::Floor1:
        return std::make_unique<StaticBlock>(Block::Type::Floor1, position);
    case Block::Type::UnderFloor1:
        return std::make_unique<StaticBlock>(Block::Type::UnderFloor1, position);
    case Block::Type::StaticBlock1:
        return std::make_unique<StaticBlock>(Block::Type::StaticBlock1, position);
    case Block::Type::Breakable1:
        return std::make_unique<BreakableBlock>(Block::Type::Breakable1, position);
    case Block::Type::LuckyBlock1:
        return std::make_unique<LuckyBlock>(Block::Type::LuckyBlock1, position);
    case Block::Type::Floor2:
        return std::make_unique<StaticBlock>(Block::Type::Floor2, position);
    case Block::Type::UnderFloor2:
        return std::make_unique<StaticBlock>(Block::Type::UnderFloor2, position);
    case Block::Type::StaticBlock2:
        return std::make_unique<StaticBlock>(Block::Type::StaticBlock2, position);
    case Block::Type::Breakable2:
        return std::make_unique<BreakableBlock>(Block::Type::Breakable2, position);
    case Block::Type::LuckyBlock2:
        return std::make_unique<LuckyBlock>(Block::Type::LuckyBlock2, position);
    case Block::Type::SlideBlock:
        return std::make_unique<SlideBlock>(Block::Type::SlideBlock, position);
    case Block::Type::JumpyBlock:
        return std::make_unique<JumpyBlock>(Block::Type::JumpyBlock, position);
    default:
        return nullptr;
    }
}