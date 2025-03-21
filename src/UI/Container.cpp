#include <SFML/Graphics/RenderTarget.hpp>

#include "Container.hpp"
#include <iostream>



namespace GUI
{
    Container::Container()
    : nChildren()
    , nSelectedChild(-1)
    {
    }

    void Container::pack(Component::Ptr component, bool isAuto) {
        nChildren.push_back(component);
        nIsAuto.push_back(isAuto);
        if (!hasSelection() && component->isSelectable()) {
            select(nChildren.size() - 1);
        }
    }

    void Container::update(sf::Time dt) {
        for (const Component::Ptr& child : nChildren) {
            child->update(dt);
        }
    }

    bool Container::isSelectable() const { return false; }

    bool Container::isPressasble() const {
        return false;
    }

    void Container::handleEvent(const sf::Event& event) {
        if (hasSelection() && nChildren[nSelectedChild]->isActive()) {
            std::cerr << "Container handle event" << ' ' << nSelectedChild << std::endl;
            nChildren[nSelectedChild]->handleEvent(event);
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                selectPrevious();
            }
            else if (event.key.code == sf::Keyboard::Right) {
                selectNext();
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                if (hasSelection()) {
                    nChildren[nSelectedChild]->activate();
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                for (std::size_t i = 0; i < nChildren.size(); ++i) if (nChildren[i]->isPressable()) {
                    if (nChildren[i]->contains(mousePos) == true) {
                        select(i);
                        nChildren[i]->activate();
                        break;
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            for (std::size_t i = 0; i < nChildren.size(); ++i) if (nChildren[i]->isSelectable()) {
                if (nChildren[i]->contains(mousePos) == true) {
                    select(i);
                    break;
                }
            }
        }
    }

/*
    Component::Ptr Container::getChildMouseOver(sf::RenderWindow &window)
    {
	    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Transform transform = getTransform();
        for (std::size_t i = 0; i < nChildren.size(); ++i) if (nChildren[i]->isSelectable()) {
            if (nChildren[i]->isMouseOver(window)) {
                return nChildren[i];
            }
        }
    }
*/
    void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        for (const Component::Ptr& child : nChildren) {
            target.draw(*child, states);
        }
    }

    void Container::select(std::size_t index) {
        if (nChildren[index]->isSelectable()) {
            if (hasSelection()) {
                nChildren[nSelectedChild]->deselect();
            }
            nChildren[index]->select();
            if (nIsAuto[index]) {
                nChildren[index]->activate();
            }
            nSelectedChild = index;
        }
    }

    void Container::selectNext() {
        if (!hasSelection()) {
            return;
        }

        int next = nSelectedChild;
        do {
            next = (next + 1) % nChildren.size();
        } while (!nChildren[next]->isSelectable());

        select(next);
    }

    void Container::selectPrevious() {
        if (!hasSelection()) {
            return;
        }

        int prev = nSelectedChild;
        do {
            prev = (prev + nChildren.size() - 1) % nChildren.size();
        } while (!nChildren[prev]->isSelectable());

        select(prev);
    }

    bool Container::hasSelection() const {
        return nSelectedChild >= 0;
    }

    Component::Ptr Container::getSelectedChild() const {
        if (nSelectedChild == -1) return nullptr;
        return nChildren[nSelectedChild];
    }
    int Container::getSelectedIndex() const {
        return nSelectedChild;
    }
}