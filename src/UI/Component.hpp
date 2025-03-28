#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

namespace GUI
{
    class Component : public sf::Drawable
                    , public sf::Transformable
                    , private sf::NonCopyable
    {
        public:
            typedef std::shared_ptr<Component> Ptr;
        public:
            Component();
            virtual ~Component();
            virtual bool isSelectable() const = 0;
            bool isSelected() const;
            virtual void update(sf::Time dt);
            virtual void select();
            virtual void deselect();
            virtual bool isPressable() const;
            virtual bool isActive() const;
            virtual void activate();
            virtual void deactivate();
            virtual void handleEvent(const sf::Event& event) = 0;
            virtual bool contains(const sf::Vector2f& point) const;
            virtual bool isMouseOver(sf::RenderWindow& window) const;
        private:
            bool nIsSelected;
            bool nIsActive;
            bool nIsPressable;

    };
}