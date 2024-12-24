#include "CreativeState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>


CreativeState::CreativeState(StateStack& stack, Context context)
: State(stack, context),
nView(context.window->getDefaultView())
{
    nView.setSize(800, 600);
    nView.setCenter(400, 300);
    context.window->setView(nView);
    context.window->setKeyRepeatEnabled(true);

    auto paintButton = std::make_shared<GUI::Button>(context, Textures::ID::PaintButtonNormal, Textures::ID::PaintButtonSelected, Textures::ID::PaintButtonPressed);
    paintButton->setPosition(79, 162);
    paintButton->setCallback([this] ()
    {
        nCursor = Cursor::Paint;
    });

    auto eraseButton = std::make_shared<GUI::Button>(context, Textures::ID::EraseButtonNormal, Textures::ID::EraseButtonSelected, Textures::ID::EraseButtonPressed);
    eraseButton->setPosition(79, 224);
    eraseButton->setCallback([this] ()
    {
        nCursor = Cursor::Erase;
    });

    auto handButton = std::make_shared<GUI::Button>(context, Textures::ID::HandButtonNormal, Textures::ID::HandButtonSelected, Textures::ID::HandButtonPressed);
    handButton->setPosition(79, 291);
    handButton->setCallback([this] ()
    {
        nCursor = Cursor::Hand;
    });

    auto undoButton = std::make_shared<GUI::Button>(context, Textures::ID::UndoButtonNormal, Textures::ID::UndoButtonSelected, Textures::ID::UndoButtonPressed);
    undoButton->setPosition(79, 356);
    undoButton->setCallback([this] ()
    {
        // nWorld.undo();
    });

    auto redoButton = std::make_shared<GUI::Button>(context, Textures::ID::RedoButtonNormal, Textures::ID::RedoButtonSelected, Textures::ID::RedoButtonPressed);
    redoButton->setPosition(79, 425);
    redoButton->setCallback([this] ()
    {
        // nWorld.redo();
    });
        
    nBarControl.pack(paintButton);
    nBarControl.pack(eraseButton);
    nBarControl.pack(handButton);
    nBarControl.pack(undoButton);
    nBarControl.pack(redoButton);
}


void CreativeState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(nView);
    
    for (int i = 0; i < MAX_X; i += BLOCK_SIZE)
    {
        for (int j = 0; j < MAX_Y; j += BLOCK_SIZE)
        {
            sf::RectangleShape block;
            block.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
            block.setPosition(i, j);
            block.setFillColor(sf::Color(0, 0, 0, 50));
            block.setOutlineColor(sf::Color(255, 255, 255, 50));
            block.setOutlineThickness(2);
            window.draw(block);
        }
    }
    window.draw(nBarControl);
}

bool CreativeState::update(sf::Time dt)
{
    return true;
}

bool CreativeState::handleEvent(const sf::Event& event)
{   
    sf::RenderWindow& window = *getContext().window;
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code)
        {
            case sf::Keyboard::Escape:
                requestStackPop();
                break;
            case sf::Keyboard::Left:
                if (nView.getCenter().x - nView.getSize().x / 2 > -100)
                    nView.move(-10, 0);
                break;
            case sf::Keyboard::Right:
                if (nView.getCenter().x + nView.getSize().x / 2 < MAX_X + 100)
                    nView.move(10, 0);
                break;
            case sf::Keyboard::Up:
                if (nView.getCenter().y - nView.getSize().y / 2 > -100)
                    nView.move(0, -10);
                break;
            case sf::Keyboard::Down:
                if (nView.getCenter().y + nView.getSize().y / 2 < MAX_Y + 100)
                    nView.move(0, 10);
                break;
            default:
                break;
        }
    }
    nBarControl.handleEvent(event);
    return false;
}