#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../World/Animation.hpp"
#include "../StateStack/StateStack.hpp"
#include "Component.hpp"
#include "../Holder/ResourceHolder.hpp"

#include <functional>

namespace GUI{
    class Button : public Component {

        public:
        typedef std::function<void()> Callback;
        typedef std::shared_ptr<Button> Ptr;
        
        Button(State::Context context, Textures::ID normal, Textures::ID selected, Textures::ID pressed);

        void setNormalTexture(Textures::ID);
        void setSelectedTexture(Textures::ID);
        void setPressedTexture(Textures::ID);
        void setNormalSprite();
        void setSelectedSprite();
        void setPressedSprite();
        void setCallback(Callback callback);
        void setText(const std::string& text);
        void setColor(const sf::Color& color);
        void setSpriteColor(const sf::Color& color);
        void setSizeText(unsigned int size);
        void setToggle(bool flag);

        void setIsSelected(bool flag);
        void setIsPressable(bool flag);

        virtual void update(sf::Time dt);
        virtual bool isSelectable() const;
        virtual bool isPressable() const;
        virtual void select();
        virtual void deselect();
        virtual bool contains(const sf::Vector2f& point) const;

        virtual void activate();
        virtual void deactivate();

        void setButtonSize(float width, float height);

        virtual void handleEvent(const sf::Event& event);
        bool isMouseOver(sf::RenderWindow &window);
        private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
        Callback nCallback;
        sf::Texture nNormalTexture;
        sf::Texture nSelectedTexture;
        sf::Texture nPressedTexture;
        SoundPlayer& nSoundPlayer;
        bool nIsSelected = true;
        bool nIsPressable = true;
        bool nIsSelecting = false;
        sf::Sprite nSprite;
        Animation nSelectedAni;
        sf::Text nText;
        bool nIsToggle;
        sf::Vector2f nButSize;
    };
}