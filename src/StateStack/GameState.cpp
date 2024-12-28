#include "GameState.hpp"
#include <filesystem>
#include <string>
#include <fstream>
#include "../Utility.hpp"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, nWorld(*context.window)
, nPlayer(*context.player)
{

    saveCurrentState();
    // nWorld.load();
    // nWorld.loadMap();
    std::ifstream fileLevel(*context.saveFile + "level.bin", std::ios::binary);
    std::ifstream fileData(*context.saveFile + "data.bin", std::ios::binary);
    if (!fileData)
    {
        int level;
        fileLevel.read((char*)&level, sizeof(int));
        nWorld.loadMap(std::to_string(level));
    }
    else {
        int level;
        fileLevel.read((char*)&level, sizeof(int));
        --level;
        nWorld.load(fileData, level);
    }
}

void GameState::draw()
{
    nWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    if (nWorld.isWin())
    {
        requestStackPop();
        requestStackPush(States::ID::Winning);
        requestStackPush(States::ID::Transition);
        return true;
    }
    if (nWorld.isLose())
    {
        requestStackPop();
        requestStackPush(States::ID::Losing);
        return true;
    }
    nWorld.update(dt);

    CommandQueue& commands = nWorld.getCommandQueue();
    nPlayer.handleRealtimeInput(commands);

    return false;
}

bool GameState::handleEvent(const sf::Event& event)
{
    if (nWorld.isWin())
    {
        requestStackPop();
        requestStackPush(States::ID::Winning);
        requestStackPush(States::ID::Transition);
        return true;
    }
    if (nWorld.isLose())
    {
        requestStackPop();
        requestStackPush(States::ID::Losing);
        return true;
    }
    CommandQueue& commands = nWorld.getCommandQueue();
    nPlayer.handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPush(States::ID::Pause);
        std::ofstream savefile("file/CurSave/temp", std::ios::binary);
        nWorld.save(savefile);
        savefile.close();
    }
    return true;
}

void GameState::saveCurrentState() {
    Context context = getContext();
    std::ofstream savefile(*context.saveFile + "state.bin", std::ios::binary);
    assert(savefile.is_open());
    int state = States::ID::Game;
    savefile.write((char*)&state, sizeof(int));
    savefile.close();
}
