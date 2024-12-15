#include "Game.hpp"
#include <iostream>
#include <stdexcept>
#include "Holder/ResourceHolder.hpp"

int main() 
{	
    try
	{
		Game game;
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}
