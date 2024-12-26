#include <SFML/Graphics/RenderTarget.hpp>
#include "VolumeBar.hpp"
#include <iostream>



namespace GUI
{
    VolumeBar::VolumeBar(State::Context context)
    : currentBlock(2)
    , increaseButton(context, Textures::ID::VolumeIncreaseButtonNormal, Textures::ID::VolumeIncreaseButtonSelected, Textures::ID::VolumeIncreaseButtonPressed)
    , decreaseButton(context, Textures::ID::VolumeDecreaseButtonNormal, Textures::ID::VolumeDecreaseButtonSelected, Textures::ID::VolumeDecreaseButtonPressed)
    {
        increaseButton.setCallback([this] ()
        {
            increaseVolume();
        });
        decreaseButton.setCallback([this] ()
        {
            decreaseVolume();
        });
        decreaseButton.setPosition(40, 41);
        increaseButton.setPosition(425, 41);
        for (int i = 0; i < MAX_BLOCK; i++) {
            normalBlock[i].setPosition(START_X + i * BLOCK_SIZE, START_Y);
        }
        for (int i = 0; i < MAX_BLOCK; i++) {
            normalBlock[i].setTexture(TextureHolder::getInstance().get(Textures::ID::VolumeBlock));
        }
        
    }

    void VolumeBar::update(sf::Time dt) {}

    void VolumeBar::increaseVolume() {
        if (currentBlock < MAX_BLOCK) {
            currentBlock++;
        }
    }

    void VolumeBar::decreaseVolume() {
        if (currentBlock > 0) {
            currentBlock--;
        }
    }

    float VolumeBar::getVolume() const {
        return currentBlock / static_cast<float>(MAX_BLOCK) * 100.0f;
    }

    void VolumeBar::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                mousePos = getInverseTransform().transformPoint(mousePos);
                if (increaseButton.contains(mousePos)) {
                    increaseButton.activate();
                }
                else if (decreaseButton.contains(mousePos)) {
                    decreaseButton.activate();
                }
            }
        }
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            mousePos = getInverseTransform().transformPoint(mousePos);
            if (increaseButton.contains(mousePos)) {
                increaseButton.select();
            }
            else {
                increaseButton.deselect();
            }
            if (decreaseButton.contains(mousePos)) {
                decreaseButton.select();
            }
            else {
                decreaseButton.deselect();
            }
        }
    }


    void VolumeBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(increaseButton, states);
        target.draw(decreaseButton, states);
        for (int i = 0; i < currentBlock; i++) {
            target.draw(normalBlock[i], states);
        }
    }
    bool VolumeBar::isSelectable() const {
        return false;
    }
    void VolumeBar::setVolume(float volume) {
        currentBlock = volume / 100.0f * MAX_BLOCK;
    }
}  // namespace GUI
