// #pragma once
// #include "BouncingBlock.hpp"
// #include "StaticBlock.hpp"
// #include "Animation.hpp"
// #include <SFML/Graphics/Sprite.hpp>
// #include <stack>
// #include "Animation.hpp"

// class Item;
// class LuckyBlock : virtual public BouncingBlock, virtual public StaticBlock
// {
//     public:
//         LuckyBlock(Type type, sf::Vector2f position);
//         virtual void applyNormal(SceneNode& graph);
//         virtual sf::FloatRect getBoundingRect() const;
//         // void dropItem(Item* item);

//     protected:
//         virtual void updateCurrent(sf::Time dt);
//         virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        

//     private:
//         bool nIsEmpty;
//         sf::Sprite nStaticSprite;
//         std::stack<Item*> nItems;
// };