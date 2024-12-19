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

    Textures::ID Button::getNormalTexture(Type type) const {
        if (type == Type::RectangleButton) {
            return Textures::RectButtonNormal;
        }
        if (type == Type::SquareButton) {
            return Textures::SquareButtonNormal;
        }
        if (type == Type::BackButton) {
            return Textures::BackButtonNormal;
        }
        if (type == Type::charSlot1) {
            return Textures::charSlot1Normal;
        }
        if (type == Type::charSlot2) {
            return Textures::charSlot2Normal;
        }
        if (type == Type::previousButton) {
            return Textures::previousButton;
        }
        if (type == Type::nextButton) {
            return Textures::nextButton;
        }
        if (type == Type::saveButton) {
            return Textures::saveButtonNormal;
        }
         if (type == Type::choosePlayerButton) {
            return Textures::choosePlayerButtonNormal;
        }
        if (type == Type::chooseCharButton) {
            return Textures::chooseCharButtonNormal;
        }
        if(type == Type::chooseModeButton) {
            return Textures::chooseModeButtonNormal;
        }
        if (type == Type::playStartButton) {
            return Textures::playStartButtonNormal;
        }
        if (type == Type::kitchenMode) {
            return Textures::kitchenModeNormal;
        }
        if (type == Type::hallwayMode) {
            return Textures::hallwayModeNormal;
        }
        if (type == Type::gardenMode) {
            return Textures::gardenModeNormal;
        }
        if (type == Type::creativeMode) {
            return Textures::creativeModeNormal;
        }
        if (type == Type::onePlayerButton) {
            return Textures::onePlayerButton;
        }
        if (type == Type::twoPlayerButton) {
            return Textures::twoPlayerButton;
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
        if (type == Type::BackButton) {
            return Textures::BackButtonSelected;
        }
         if (type == Type::charSlot1) {
            return Textures::charSlot1Selected;
        }
        if (type == Type::charSlot2) {
            return Textures::charSlot2Selected;
        }
         if (type == Type::previousButton) {
            return Textures::previousButton;
        }
        if (type == Type::nextButton) {
            return Textures::nextButton;
        }
        if (type == Type::saveButton) {
            return Textures::saveButtonSelected;
        }
        if (type == Type::choosePlayerButton) {
            return Textures::choosePlayerButtonSelected;
        }
        if (type == Type::chooseCharButton) {
            return Textures::chooseCharButtonSelected;
        }
        if (type == Type::chooseModeButton) {
            return Textures::chooseModeButtonSelected;
        }
        if (type == Type::playStartButton) {
            return Textures::playStartButtonSelected;
        }
        if (type == Type::kitchenMode) {
            return Textures::kitchenModeSelected;
        }
        if (type == Type::hallwayMode) {
            return Textures::hallwayModeSelected;
        }
        if (type == Type::gardenMode) {
            return Textures::gardenModeSelected;
        }
        if (type == Type::creativeMode) {
            return Textures::creativeModeSelected;
        }
        if (type == Type::onePlayerButton) {
            return Textures::onePlayerButton;
        }
        if (type == Type::twoPlayerButton) {
            return Textures::twoPlayerButton;
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
        if (type == Type::BackButton) {
            return Textures::BackButtonPressed;
        }
         if (type == Type::charSlot1) {
            return Textures::charSlot1Pressed;
        }
        if (type == Type::charSlot2) {
            return Textures::charSlot2Pressed;
        }
         if (type == Type::previousButton) {
            return Textures::previousButton;
        }
        if (type == Type::nextButton) {
            return Textures::nextButton;
        }
        if (type == Type::saveButton) {
            return Textures::saveButtonSelected;
        }
         if (type == Type::choosePlayerButton) {
            return Textures::choosePlayerButtonSelected;
        }
        if (type == Type::chooseCharButton) {
            return Textures::chooseCharButtonSelected;
        }
        if (type == Type::chooseModeButton) {
            return Textures::chooseModeButtonSelected;
        }
        if (type == Type::playStartButton) {
            return Textures::playStartButtonSelected;
        }
        if (type == Type::kitchenMode) {
            return Textures::kitchenModeSelected;
        }
        if (type == Type::hallwayMode) {
            return Textures::hallwayModeSelected;
        }
        if (type == Type::gardenMode) {
            return Textures::gardenModeSelected;
        }
        if (type == Type::creativeMode) {
            return Textures::creativeModeSelected;
        }
        if (type == Type::onePlayerButton) {
            return Textures::onePlayerButton;
        }
        if (type == Type::twoPlayerButton) {
            return Textures::twoPlayerButton;
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
}