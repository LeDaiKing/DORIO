#include "Button.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include "../Music/SoundPlayer.hpp"
#include <cmath>
#include <iostream>


namespace GUI{
    Button::Button(State::Context context, Textures::ID normal, Textures::ID selected, Textures::ID pressed) 
    : nCallback()
    , nNormalTexture(TextureHolder::getInstance().get(normal))
    , nSelectedTexture(TextureHolder::getInstance().get(selected))
    , nSelectedAni(TextureHolder::getInstance().get(selected))
    , nPressedTexture(TextureHolder::getInstance().get(pressed))
    , nText("", FontHolder::getInstance().get(Fonts::Main), 16)
    , nIsToggle(false)
    , nSoundPlayer(*context.sounds)
    , nButSize(-1, -1)
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
      if (nButSize.x != -1 && nButSize.y != -1) {
        nSprite.setScale(nButSize.x / nSprite.getLocalBounds().width, nButSize.y / nSprite.getLocalBounds().height);
      }
      // sf::FloatRect bounds = nSprite.getLocalBounds();
      // nText.setPosition(std::floor(bounds.left + bounds.width / 2.f),
      // std::floor(bounds.top + bounds.height / 2.f));
    }

    void Button::setSelectedSprite() {
        nSprite.setTexture(nSelectedTexture, true);
        centerOrigin(nSprite);
        if (nButSize.x != -1 && nButSize.y != -1) {
            nSprite.setScale(nButSize.x / nSprite.getLocalBounds().width, nButSize.y / nSprite.getLocalBounds().height);
        }
        // sf::FloatRect bounds = nSprite.getLocalBounds();
        // std::cerr << "Bounds: " << bounds.left << " " << bounds.top << " " << bounds.width << " " << bounds.height << std::endl;
        // nText.setPosition(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));        
    }

    void Button::setPressedSprite() {
        nSprite.setTexture(nPressedTexture, true);
        centerOrigin(nSprite);
        if (nButSize.x != -1 && nButSize.y != -1) {
            nSprite.setScale(nButSize.x / nSprite.getLocalBounds().width, nButSize.y / nSprite.getLocalBounds().height);
        }
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

    void Button::setSpriteColor(const sf::Color& color) {
        nSprite.setColor(color);
    }

    void Button::setSizeText(unsigned int size) {
        nText.setCharacterSize(size);
        centerOrigin(nText);
    }

    void Button::setToggle(bool flag) {
        nIsToggle = flag;
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
        //nIsSelecting = true;
        Component::select();
        setSelectedSprite();
    }

    void Button::deselect() {
        nIsSelecting = false;
        Component::deselect();
        setNormalSprite();
    }

    bool Button::contains(const sf::Vector2f& point) const {
        sf::Transform transform = getTransform();
        return nSprite.getGlobalBounds().contains(transform.getInverse().transformPoint(point));
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
            sf::Transform transform = getTransform();
            sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            if (nSprite.getGlobalBounds().contains(transform.getInverse().transformPoint(mousePos))) {
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

    bool Button::isMouseOver(sf::RenderWindow &window)
    {
	    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Transform transform = getTransform();
        return nSprite.getGlobalBounds().contains(transform.getInverse().transformPoint(mousePos));
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

    void Button::setButtonSize(float width, float height) {
        nButSize = sf::Vector2f(width, height);
    }
}