#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
// #include <SFML/Graphics/Rectangleshape.hpp>

class TextRender
{
    public:
        // void draw(sf::Window& window),
        static TextRender& getInstance();
        void drawText(sf::RenderWindow& window, const char* text, sf::Vector2f position, int size = -1, bool white = true);
    
    private:
        TextRender();
        ~TextRender() = default;

        TextRender(const TextRender&) = delete;
        TextRender& operator=(const TextRender&) = delete;

        sf::IntRect getRect(char x) const;

    private:
        sf::Sprite nSheetText;
        sf::Sprite nSheetText2;
        

};