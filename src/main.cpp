#include "Game.hpp"
#include <iostream>
#include <stdexcept>
#include "Holder/ResourceHolder.hpp"
void loadTextures()
{
	TextureHolder::getInstance().load(Textures::Dirt, "res/Background/Dirt.png");
	TextureHolder::getInstance().load(Textures::Dough1, "res/Dough/dough.png");
	TextureHolder::getInstance().load(Textures::Dough2, "res/Dough/tile001.png");
	TextureHolder::getInstance().load(Textures::BigDough, "res/Dough/BigDough.png");
	TextureHolder::getInstance().load(Textures::Sky, "res/Background/bg1.png");
	TextureHolder::getInstance().load(Textures::Enemy, "res/Enemy/Enemy.png");
	TextureHolder::getInstance().load(Textures::Ghost, "res/Enemy/Ghost/Ghost.png");
	TextureHolder::getInstance().load(Textures::Chicken, "res/Enemy/Chicken/Chicken.png");
	TextureHolder::getInstance().load(Textures::CockRoach, "res/Enemy/CockRoach/CockRoach.png");
	TextureHolder::getInstance().load(Textures::Snail, "res/Enemy/Snail/Snail.png");
	TextureHolder::getInstance().load(Textures::SnailShell, "res/Enemy/Snail/SnailShell.png");
	TextureHolder::getInstance().load(Textures::Breakable, "res/Background/Breakable/Breakable.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation, "res/Background/Breakable/BreakAnimation.png");
	TextureHolder::getInstance().load(Textures::LuckyBlock, "res/Background/LuckyBlock/LuckyBlock.png");
	TextureHolder::getInstance().load(Textures::StaticLuckyBlock, "res/Background/LuckyBlock/StaticLuckyBlock.png");
	TextureHolder::getInstance().load(Textures::JumpyBlock, "res/Background/JumpyBlock/JumpyBlock.png");
	TextureHolder::getInstance().load(Textures::Coin, "res/Item/Coin.png");
	TextureHolder::getInstance().load(Textures::CoinAnimation, "res/Item/CoinAnimation.png");
	TextureHolder::getInstance().load(Textures::Heart, "res/Item/Heart.png");
	TextureHolder::getInstance().load(Textures::HeartAnimation, "res/Item/StarAnimation.png");
	TextureHolder::getInstance().load(Textures::FireBall, "res/Item/Fireball.png");
	TextureHolder::getInstance().load(Textures::HeartIcon, "res/Hub/HeartIcon.png");
	TextureHolder::getInstance().load(Textures::CoinIcon, "res/Hub/CoinIcon.png");
	TextureHolder::getInstance().load(Textures::ClockIcon, "res/Hub/ClockIcon.png");
	TextureHolder::getInstance().load(Textures::SettingBut, "res/Buttons/Settings.png");
	TextureHolder::getInstance().load(Textures::WhiteText, "res/Text/WhiteText.png");
	TextureHolder::getInstance().load(Textures::BlackText, "res/Text/BlackText.png");
}

int main() 
{	
	loadTextures();
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
