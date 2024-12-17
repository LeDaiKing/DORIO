#include "Button.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include "../Music/SoundPlayer.hpp"
#include <cmath>
#include <iostream>


namespace GUI{
    Button::Button(State::Context context, Type type = Type::RectangleButton) 
    : nCallback()
    , nNormalTexture(TextureHolder::getInstance().get(getNormalTexture(type)))
    , nSelectedTexture(TextureHolder::getInstance().get(getSelectedTexture(type)))
    , nSelectedAni(TextureHolder::getInstance().get(getSelectedTexture(type)))
    , nPressedTexture(TextureHolder::getInstance().get(getPressedTexture(type)))
    , nText("", FontHolder::getInstance().get(Fonts::Main), 16)
    , nIsToggle(false)
    , nType(type)
    , nSoundPlayer(*context.sounds)
    {
        nSelectedAni.addAnimationState(0, 0, 2, sf::seconds(1.0f), sf::Vector2i(486, 274), true);
        nSelectedAni.setAnimationState(0);
        setNormalSprite();
        // sf::FloatRect bounds = nSprite.getLocalBounds();
        // nText.setPosition(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));        
    }


    void Button::setNormalSprite() {
      nSprite.setTexture(nNormalTexture, true);
      centerOrigin(nSprite);
      // sf::FloatRect bounds = nSprite.getLocalBounds();
      // nText.setPosition(std::floor(bounds.left + bounds.width / 2.f),
      // std::floor(bounds.top + bounds.height / 2.f));
    }

    void Button::setSelectedSprite() {
        nSprite.setTexture(nSelectedTexture, true);
        centerOrigin(nSprite);
        // sf::FloatRect bounds = nSprite.getLocalBounds();
        // std::cerr << "Bounds: " << bounds.left << " " << bounds.top << " " << bounds.width << " " << bounds.height << std::endl;
        // nText.setPosition(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));        
    }

    void Button::setPressedSprite() {
        nSprite.setTexture(nPressedTexture, true);
        centerOrigin(nSprite);
        // sf::FloatRect bounds = nSprite.getLocalBounds();
        // nText.setPosition(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));        
    }

    void Button::setCallback(Callback callback) {
        nCallback = std::move(callback);
    }

    void Button::setText(const std::string& text) {
        nText.setString(text);
        centerOrigin(nText);
    }

    void Button::setColor(const sf::Color& color) {
        nText.setFillColor(color);
    }

    void Button::setSizeText(unsigned int size) {
        nText.setCharacterSize(size);
        centerOrigin(nText);
    }

    void Button::setToggle(bool flag) {
        nIsToggle = flag;
    }

    Textures::ID Button::getNormalTexture(Type type) const {
        if (type == Type::RectangleButton) {
            return Textures::RectButtonNormal;
        }
        if (type == Type::SquareButton) {
            return Textures::SquareButtonNormal;
        }
        if (type == Type::CreativeButton) {
            return Textures::CreativeButtonNormal;
        }
        if (type == Type::BackButton) {
            return Textures::BackButtonNormal;
        }

        assert(false);
    }

    Textures::ID Button::getSelectedTexture(Type type) const {
        if (type == Type::RectangleButton) {
            return Textures::RectButtonSelected;
        }
        if (type == Type::SquareButton) {
            return Textures::SquareButtonSelected;
        }
        if (type == Type::CreativeButton) {
            return Textures::CreativeButtonSelected;
        }
        if (type == Type::BackButton) {
            return Textures::BackButtonSelected;
        }

        assert(false);
    }

    Textures::ID Button::getPressedTexture(Type type) const {
        if (type == Type::RectangleButton) {
            return Textures::RectButtonPressed;
        }
        if (type == Type::SquareButton) {
            return Textures::SquareButtonPressed;
        }
        if (type == Type::CreativeButton) {
            return Textures::CreativeButtonPressed;
        }
        if (type == Type::BackButton) {
            return Textures::BackButtonPressed;
        }
        assert(false);
    }

    void Button::setIsSelected(bool flag) {
        nIsSelected = flag;
    }

    void Button::update(sf::Time dt) {
        if (nIsSelecting) {
            nSelectedAni.update(dt);
        }
    }

    bool Button::isSelectable() const { return nIsSelected; }

    bool Button::isPressable() const {
        return true;
    }

    void Button::select() {
        nIsSelecting = true;
        Component::select();
        setSelectedSprite();
    }

    void Button::deselect() {
        nIsSelecting = false;
        Component::deselect();
        setNormalSprite();
    }

    bool Button::contains(const sf::Vector2f& point) const {
        return getTransform().transformRect(nSprite.getGlobalBounds()).contains(point);
    }

    void Button::activate() {
        std::cerr << "Button activated" << std::endl;

        Component::activate();
        if (nIsToggle) {
            setPressedSprite();
        }
        if (nCallback) {
            nCallback();
        }
        if (!nIsToggle) {
            deactivate();
        }
        nSoundPlayer.play(SoundEffect::ClickedButton);
    }

    void Button::deactivate() {
        Component::deactivate();
        if (isSelected()) {
            setSelectedSprite();
        }
        else {
            setNormalSprite();
        }
    }

    void Button::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            if (nSprite.getGlobalBounds().contains(mousePos)) {
                select();
            }
            else {
                deselect();
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Transform transform = getTransform();
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                if (nSprite.getGlobalBounds().contains(transform.getInverse().transformPoint(mousePos))) {
                    activate();
                }
            }
        }
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        if (nIsSelecting) {
            target.draw(nSelectedAni, states);
        }
        else {
            target.draw(nSprite, states);
        }
        target.draw(nText, states);
    }
}