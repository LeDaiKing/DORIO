#include "TextBox.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Utility.hpp"
#include "../Music/SoundPlayer.hpp"
#include <cmath>
#include <iostream>



namespace GUI{
    TextBox::TextBox(State::Context context)
    : nSprite(TextureHolder::getInstance().get(Textures::ID::TextBox))
    , nText("")
    , nInput("")
    , nInputWithCursor("|")
    , nClock()
    {
        nText.setCharacterSize(30);
        nText.setColor(sf::Color::White);
        nText.setPosition(100, -50);

        nInput.setCharacterSize(30);
        nInput.setColor(sf::Color::Black);
        nInput.setPosition(100, 33.67);

        nInputWithCursor.setPosition(100, 33.67);
        nInputWithCursor.setCharacterSize(30);
        nInputWithCursor.setColor(sf::Color::Black);
        nClock.restart();
    }

    TextBox::~TextBox() {}

    bool TextBox::isSelectable() const {
        return false;
    }

    void TextBox::update(sf::Time dt) {
        if (nClock.getElapsedTime().asMilliseconds() > TIME_CURSOR) {
            nClock.restart();
            std::string temp = nInputWithCursor.getText();
            if (temp.size() > 0 && temp[temp.size() - 1] == '|') {
                temp.pop_back();
                nInputWithCursor.setText(temp);
            }
            else {
                temp += "|";
                nInputWithCursor.setText(temp);
            }
        }
    }

    void TextBox::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            mousePos = getTransform().getInverse().transformPoint(mousePos);
            std::cout << mousePos.x << " " << mousePos.y << std::endl;
            std::cout << nSprite.getGlobalBounds().left << " " << nSprite.getGlobalBounds().top << std::endl;
            if (nSprite.getGlobalBounds().contains(mousePos)) {
                setChoose(true);
            }
            else {
                setChoose(false);
            }
        }
        if (isChoose == true) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    setChoose(false);
                }
                else if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                    std::string temp = nInput.getText();
                    temp += static_cast<char>(event.key.code - sf::Keyboard::A + 'A');
                    setInput(temp);
                }
                else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
                    std::string temp = nInput.getText();
                    temp += static_cast<char>(event.key.code - sf::Keyboard::Num0 + '0');
                    setInput(temp);
                }
                else if (event.key.code == sf::Keyboard::BackSpace) {
                    std::string temp = nInput.getText();
                    if (temp.size() > 0) {
                        temp.pop_back();
                        setInput(temp);
                    }
                }
            }
        }

    }
    void TextBox::setTitle(const std::string& text) {
        nText.setText(text);
    } 

    void TextBox::setInput(const std::string& text) {
        nInput.setText(text);
        std::string temp = text;
        std::string input = nInputWithCursor.getText();
        if (input.size() > 0 && input[input.size() - 1] == '|') {
            temp += "|";
        }
        nInputWithCursor.setText(temp);
    }

    void TextBox::setChoose(bool flag) {
        isChoose = flag;
    }

    std::string TextBox::getInput() const {
        return nInput.getText();
    }

    void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        const std::string input = nInput.getText();
        states.transform *= getTransform();
        target.draw(nSprite, states);
        target.draw(nText, states);
        if (isChoose) {
            target.draw(nInputWithCursor, states);
        }
        else {
            target.draw(nInput, states);
        }
    }
}  // namespace GUI