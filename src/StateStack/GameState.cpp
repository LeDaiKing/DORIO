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

    // nWorld.load();
    // nWorld.loadMap();
    std::ifstream file("file/CurSave/save.txt");
    file >> saveFile;
    file.close();
    if (isFileEmpty(saveFile))
    {
        nWorld.loadMap(std::string(1, saveFile[saveFile.size() - 1]));
    }
    else
    {
        std::ifstream savefile(saveFile, std::ios::binary);
        nWorld.load(savefile, saveFile[saveFile.size() - 1] - '1');
        savefile.close();
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