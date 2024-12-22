#include "Label.hpp"
#include "../Utility.hpp"

namespace GUI{
    Label::Label(const std::string& text)
    : nText(text, FontHolder::getInstance().get(Fonts::Main), 16)
    {
        centerOrigin(nText);
    }

    bool Label::isSelectable() const {
        return false;
    }

    void Label::setColor(const sf::Color& color) {
        nText.setFillColor(color);
    }

    void Label::setCharacterSize(unsigned int size) {
        nText.setCharacterSize(size);
        centerOrigin(nText);
    }

    void Label::setText(const std::string& text) {
        nText.setString(text);
        centerOrigin(nText);
    }

    float Label::getWidth() const {
        return nText.getLocalBounds().width;
    }

    float Label::getHeight() const {
        return nText.getLocalBounds().height;
    }

    void Label::handleEvent(const sf::Event& event) {
    }

    void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(nText, states);
    }
}