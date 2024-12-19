#include "Item.hpp"
#include "Animation.hpp"

class ItemFactory
{
    public:
        static std::unique_ptr<Item> createItem(Item::Type type, sf::Vector2f position);
        static std::unique_ptr<Animation> createAppearAnimation(Item::Type type);
    private:
        ItemFactory() = default;
        ~ItemFactory() = default;
        ItemFactory(const ItemFactory&) = delete;
        ItemFactory& operator=(const ItemFactory&) = delete;
};