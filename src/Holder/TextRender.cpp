#include "TextRender.hpp"
#include <iostream>
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
TextRender::TextRender()
: nSheetText(TextureHolder::getInstance().get(Textures::WhiteText))
, nSheetText2(TextureHolder::getInstance().get(Textures::BlackText))
{
}

TextRender& TextRender::getInstance()
{
    static TextRender instance;
    return instance;
}

void TextRender::drawText(sf::RenderWindow& window, const char* text, sf::Vector2f position, int size, bool white)
{
    // size = -1;
    sf::Sprite& sprite = white ? nSheetText : nSheetText2;
    sprite.setPosition(position);
     if (size != -1)
    {
        sprite.setScale(size / 8.f, size / 8.f);
    }
    for (int i = 0; text[i] != '\0'; i++)
    {
        sf::IntRect rect = getRect(text[i]);
        // if (size != -1)
        // {
        //     rect *= size / 8;
        // }
        sprite.setTextureRect(rect);
       
        window.draw(sprite);
       
       if (size != -1)
        sprite.move(7.f * size / 8.f, 0);
        else sprite.move(7.f, 0);
    }
    if (size != -1)
    {
        sprite.setScale(1.f, 1.f);
    }
}

sf::IntRect TextRender::getRect(char x) const
{
    // return sf::IntRect(0, 0, 80, 50);
    // int 
    if ('a' <= x && x <= 'z')
    {
        int index = x - 'a';
        return sf::IntRect((index % 10) * 8, (index / 10) * 10, 8, 10);
    }

    if ('A' <= x && x <= 'Z')
    {
        int index = x - 'A';
        return sf::IntRect((index % 10) * 8, (index / 10) * 10, 8, 10);
    }

    if ('0' <= x && x <= '9')
    {
        int index = x - '0' + 30;
        return sf::IntRect((index % 10) * 8, (index / 10) * 10, 8, 10);
    }
    switch (x)
    {
    case '.':
        return sf::IntRect(0, 40, 8, 10);
    case ',':
        return sf::IntRect(8, 40, 8, 10);
    case ':':
        return sf::IntRect(16, 40, 8, 10);
    case '?':
        return sf::IntRect(24, 40, 8, 10);
    case '!':
        return sf::IntRect(32, 40, 8, 10);
    case '(':
        return sf::IntRect(40, 40, 8, 10);
    case ')':
        return sf::IntRect(48, 40, 8, 10);
    case '+':
        return sf::IntRect(56, 40, 8, 10);
    case '-':
        return sf::IntRect(64, 40, 8, 10); 
    case ' ':
        return sf::IntRect(72, 40, 8, 10);
    default:
        break;
    }
    return sf::IntRect(23, 40, 8, 10);
}