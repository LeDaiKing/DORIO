#include "Block.hpp"
#include <string>   

class BlockFactory
{
    public:
        static std::unique_ptr<Block> createBlock(const std::string& type, sf::Vector2f position);

    private:
        BlockFactory() = default;
        ~BlockFactory() = default;
        BlockFactory(const BlockFactory&) = delete;
        BlockFactory& operator=(const BlockFactory&) = delete;
};