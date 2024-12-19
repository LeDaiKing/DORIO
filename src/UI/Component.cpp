#include "Component.hpp"

namespace GUI{
    Component::Component()
    : nIsSelected(false)
    , nIsActive(false)
    {
    }
    
    Component::~Component()
    {
    }

    bool Component::isSelected() const
    {
        return nIsSelected;
    }

    void Component::update(sf::Time dt) {
        // pass
    }

    void Component::select()
    {
        nIsSelected = true;
    }

    void Component::deselect()
    {
        nIsSelected = false;
    }

    bool Component::isPressable() const {
        return false;
    }

    bool Component::isActive() const
    {
        return nIsActive;
    }

    void Component::activate()
    {
        nIsActive = true;
    }

    void Component::deactivate()
    {
        nIsActive = false;
    }
    bool Component::contains(const sf::Vector2f& point) const {
        return false;
    }
    bool Component::isMouseOver(sf::RenderWindow& window) const {
      return false;
    }
    }  // namespace GUI