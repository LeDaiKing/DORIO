#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../World/Animation.hpp"
#include "../StateStack/StateStack.hpp"
#include "Component.hpp"

#include <functional>

namespace GUI{
    class Button : public Component {
        typedef std::shared_ptr<Button> Ptr;
        typedef std::function<void()> Callback;

        public:
        enum Type{
            SquareButton, 
            RectangleButton,
            BackButton,
            charSlot1,
            charSlot2,
            previousButton,
            nextButton,
            saveButton,
            choosePlayerButton,
            chooseCharButton,
            chooseModeButton,
            playStartButton,
            kitchenMode,
            hallwayMode,
            gardenMode,
            creativeMode,
            onePlayerButton,
            twoPlayerButton,
        };
        Button(State::Context context, Type type);

        void setNormalSprite();
        void setSelectedSprite();
        void setPressedSprite();
        void setCallback(Callback callback);
        void setText(const std::string& text);
        void setColor(const sf::Color& color);
        void setSpriteColor(const sf::Color& color);
        void setSizeText(unsigned int size);
        void setToggle(bool flag);

        Textures::ID getNormalTexture(Type type) const;
        Textures::ID getSelectedTexture(Type type) const;
        Textures::ID getPressedTexture(Type type) const;

        void setIsSelected(bool flag);

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
        const sf::Texture& nNormalTexture;
        const sf::Texture& nSelectedTexture;
        const sf::Texture& nPressedTexture;
        SoundPlayer& nSoundPlayer;
        bool nIsSelected = true;
        bool nIsSelecting = false;
        Type nType;
        sf::Sprite nSprite;
        Animation nSelectedAni;
        sf::Text nText;
        bool nIsToggle;
        sf::Vector2f nButSize;
    };
}