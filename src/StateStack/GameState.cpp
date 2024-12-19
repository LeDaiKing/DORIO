#include "GameState.hpp"



GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, nWorld(*context.window)
, nPlayer(*context.player)
{
    nWorld.load();
    nWorld.loadMap();
}

void GameState::draw()
{
    nWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    nWorld.update(dt);

    CommandQueue& commands = nWorld.getCommandQueue();
    nPlayer.handleRealtimeInput(commands);

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commands = nWorld.getCommandQueue();
    nPlayer.handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::ID::Pause);
        nWorld.save();
    }
    return true;
}