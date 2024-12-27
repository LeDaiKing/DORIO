#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Label.hpp"
#include "../StateStack/State.hpp"

#include <memory>

namespace GUI
{
    class TextBox : public Component{
        public:
            typedef std::shared_ptr<TextBox> Ptr;
        public:
            TextBox(State::Context context);
            virtual ~TextBox();
            virtual bool isSelectable() const;
            virtual void update(sf::Time dt);
            virtual void handleEvent(const sf::Event& event);
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            void setTitle(const std::string& text);
            void setInput(const std::string& text);
            void setChoose(bool flag);
            std::string getInput() const;
        private:
            const float TIME_CURSOR = 500;
            sf::Sprite nSprite;
            GUI::Label nText;
            GUI::Label nInput;
            GUI::Label nInputWithCursor;
            sf::Clock nClock;
            bool isChoose = false;
    };
}