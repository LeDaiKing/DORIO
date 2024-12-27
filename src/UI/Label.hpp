#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../UI/Component.hpp"

namespace GUI{
    class Label : public Component {
        public:
        typedef std::shared_ptr<Label> Ptr;

        public:
        Label(const std::string& text);

        virtual bool isSelectable() const;
        void setColor(const sf::Color& color);
        void setCharacterSize(unsigned int size);
        void setText(const std::string& text);
        float getWidth() const;
        float getHeight() const;
        std::string getText() const;

        virtual void handleEvent(const sf::Event& event);

        private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
        sf::Text nText;
    };
}