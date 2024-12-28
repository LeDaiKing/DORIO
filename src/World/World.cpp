#include "World.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "BreakableBlock.hpp"
#include "StaticBlock.hpp"
#include "Coin.hpp"
#include "LuckyBlock.hpp"
#include "SlideBlock.hpp"
#include "JumpyBlock.hpp"
#include "CockRoach.hpp"
#include "Ghost.hpp"
#include "Chicken.hpp"
#include "Snail.hpp"
#include "CheckPoint.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include "../ConfigLoader.hpp"
#include "BlockFactory.hpp"
#include "EnemyFactory.hpp"
#include "ItemFactory.hpp"
#include "nlohmann/json.hpp"
#include "SewerPipe.hpp"


int World::nGravity = 700;

World::World(sf::RenderWindow& window)
: nWindow(window)
, nWorldView(window.getDefaultView())
, nSceneGraph()
, nSceneLayers()
, nWorldBounds(0.f, 0.f, 5000, nWorldView.getSize().y)
, nSpawnPosition(50, nWorldBounds.height - nWorldView.getSize().y / 2.f)
, nPlayerDough(nullptr)
, nTime(sf::Time::Zero)
{
	// loadTextures();
	buildScene();

	// Prepare the view
	//zoom(0.5f);
	nWorldView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	// nWorldBounds = sf::FloatRect(0.f, 0.f, 10, 10000.f);
	nWorldView.setCenter(nWorldView.getSize().x / 4.f, nWorldView.getSize().y / 2.f);
	nWorldView.zoom(0.5f);
	// nHub = Hub();
}

void World::update(sf::Time dt)
{
	nSceneGraph.setRange(nWorldView.getCenter().x);
	nTime -= dt;
	applyNormal();
	applyGravity();
	// enemiesAttackPlayer();
	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!nCommandQueue.isEmpty())
	{
		const Command& command = nCommandQueue.pop();
		for (std::size_t i = 0; i < LayerCount; ++i)
		{
			if (nCategoryLayers[i] & command.category)
				nSceneLayers[i]->onCommand(command, dt);
		}
	}
	handleCollisions();
	removeSceneNode();

	nSceneGraph.update(dt, nCommandQueue);
	// Regular update step, adapt position (correct if outside view)

	adaptCameraPosition();
	nHub.update(dt, *nPlayerDough, nTime);
}

void World::draw()
{
	nWindow.setView(nWorldView);
	nWindow.draw(nSceneGraph);
	nWindow.setView(nWindow.getDefaultView());	
	nHub.draw(nWindow);
}

CommandQueue& World::getCommandQueue()
{
	return nCommandQueue;
}

// void World::
void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		if (i == Items)
		{
			std::unique_ptr<SceneNode> layer(new SceneNode(Category::ItemScene));
			nSceneLayers[i] = layer.get();
			nCategoryLayers[i] = Category::ItemScene;
			nSceneGraph.attachChild(std::move(layer));
			continue;
		}

		if (i == Enemies)
		{
			std::unique_ptr<SceneNode> layer(new SceneNode(Category::EnemyScene));
			nSceneLayers[i] = layer.get();
			nCategoryLayers[i] = Category::EnemyScene;
			nSceneGraph.attachChild(std::move(layer));
			continue;
		}

		SceneNode::Ptr layer(new SceneNode());
		nSceneLayers[i] = layer.get();
		nCategoryLayers[i] = Category::None;

		nSceneGraph.attachChild(std::move(layer));
	}
	nCategoryLayers[Background] |= Category::Scene;
	nCategoryLayers[Map] |= Category::Block;
	nCategoryLayers[Checkpoints] |= Category::Scene;
	nCategoryLayers[Pipes] |= Category::Block;
	nCategoryLayers[Enemies] |= Category::Enemy;
	nCategoryLayers[Items] |= Category::Item;
	nCategoryLayers[Player] |= Category::PlayerDough;
	

	
	
}



void World::applyGravity()
{
	Command applyGravity;
	applyGravity.category = Category::Entity;
	applyGravity.action = derivedAction<Entity>([this] (Entity& Dough, sf::Time)
	{
		Dough.accelerate(0.f, nGravity);
	});
	
	nCommandQueue.push(applyGravity);
}

void World::adaptCameraPosition()
{
	sf::Vector2f postiion = nPlayerDough->getPosition();

	// Camera move to the right
	if (postiion.x > nWorldView.getCenter().x)
	{
		float distance = std::min(postiion.x - nWorldView.getCenter().x, nWorldBounds.width - nWorldView.getSize().x / 2.f - nWorldView.getCenter().x);
		nWorldView.move(distance, 0);
	}
	
	if (postiion.x < nWorldView.getCenter().x - 50)
	{
		float distance = std::max(postiion.x - nWorldView.getCenter().x + 50, -nWorldView.getCenter().x + nWorldView.getSize().x / 2.f);
		nWorldView.move(distance, 0);
	}

	if (postiion.y  < nWorldView.getCenter().y - 60)
	{
		float distance = std::max(postiion.y - nWorldView.getCenter().y + 60, -nWorldView.getCenter().y + nWorldView.getSize().y / 2.f);
		nWorldView.move(0, distance);
	}

	if (postiion.y > nWorldView.getCenter().y - 60 && nWorldView.getCenter().y < nWorldBounds.height - nWorldView.getSize().y / 2.f - 200)
	{
		float distance = std::min(postiion.y - nWorldView.getCenter().y + 60, nWorldBounds.height - nWorldView.getSize().y / 2.f - 200 - nWorldView.getCenter().y);
		nWorldView.move(0, distance);
	}

	if (postiion.x < 10) nPlayerDough->setPosition(10, postiion.y);
	if (postiion.x > nWorldBounds.width - 10) nPlayerDough->setPosition(nWorldBounds.width - 10, postiion.y);
	if (postiion.y < 10) nPlayerDough->setPosition(postiion.x, 10);
}

void World::loadMap(std::string level)
{
	std::string key = "Map/Level" + level;
	nlohmann::json config = ConfigLoader::getInstance().getConfig(key.c_str());


	//Time
	nTime = sf::seconds(config["Time"].get<float>());

	sf::Image map; map.loadFromFile(config["MapImage"].get<std::string>());
	nWorldBounds.width = map.getSize().x;

	//Background
	sf::Texture& texture = TextureHolder::getInstance().get(config["Background"]);
	sf::IntRect textureRect(nWorldBounds);
	texture.setRepeated(true);
	std::unique_ptr<SpriteNode> background(new SpriteNode(texture, textureRect));
	background->setPosition(nWorldBounds.left, nWorldBounds.top);
	nSceneLayers[Background]->attachChild(std::move(background));

	//Map
	for (int x = 0; x < map.getSize().x; x += 32)
	for (int y = 0; y < map.getSize().y; y += 32)
	{
		sf::Color color = map.getPixel(x + 10, y + 10);
		if (color.toInteger() == 0xFF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("Floor" + level, sf::Vector2f(x + 16, y + 16));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xE2CB36FF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("Gold", sf::Vector2f(x + 16, y + 16));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xB2B2FF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("UnderFloor" + level, sf::Vector2f(x + 16, y + 16));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xA53A00FF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("StaticBlock" + level, sf::Vector2f(x + 16, y + 16));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xF600FFFF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("LuckyBlock" + level, sf::Vector2f(x + 16, y + 16));
			LuckyBlock& luckyBlock = static_cast<LuckyBlock&>(*block);
			luckyBlock.randomItem(-1);
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xFF0000FF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("Breakable" + level, sf::Vector2f(x + 16, y + 16));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0x54E5FFFF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("SlideBlock", sf::Vector2f(x + 16, y + 3));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xAA34A0FF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("JumpyBlock", sf::Vector2f(x + 16, y + 16));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xFFFC00FF)
		{
			std::unique_ptr<Coin> coin(new Coin(Item::Coin, sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Items]->attachChild(std::move(coin));
		}
		else if (color.toInteger() == 0xFFAAD5FF)
		{
			std::unique_ptr<CheckPoint> checkpoint(new CheckPoint(CheckPoint::Checkpoint, sf::Vector2f(x + 32, y)));
			nSceneLayers[Checkpoints]->attachChild(std::move(checkpoint));
		}
		else if (color.toInteger() == 0x4B5908FF)
		{
			std::cout << "Cup here: " << x << " " << y << std::endl;
			std::unique_ptr<CheckPoint> checkpoint(new CheckPoint(CheckPoint::End, sf::Vector2f(x + 32, y)));
			nCup = checkpoint.get();
			nSceneLayers[Checkpoints]->attachChild(std::move(checkpoint));
		}
	}

	nSpawnPosition = toVector2<float>(config["StartPoint"]);
	std::unique_ptr<CheckPoint> start(new CheckPoint(CheckPoint::Start, nSpawnPosition));
	nSceneLayers[Checkpoints]->attachChild(std::move(start));




	//Enemy
	std::string enemySpawn = "EnemySpawn" + level;
	// std::cout << enemySpawn << std::endl;
	nlohmann::json enemyConfig = ConfigLoader::getInstance().getConfig(enemySpawn.c_str());

	for (auto& cockroach : enemyConfig["CockRoach"])
	{
		sf::Vector2f position = toVector2<float>(cockroach["Position"]);
		std::unique_ptr<CockRoach> enemy(new CockRoach(Enemy::CockRoach, position + sf::Vector2f(16.0f, 16.0f)));
		for (auto& ai : cockroach["AI"])
		{
			enemy->addBehavior(ai.at(0), ai.at(1), ai.at(2));
		}
		nSceneLayers[Enemies]->attachChild(std::move(enemy));
	}

	for (auto& ghost : enemyConfig["Ghost"])
	{
		std::unique_ptr<Ghost> enemy(new Ghost(Enemy::Ghost, toVector2<float>(ghost["Position"]) + sf::Vector2f(16.0f, 16.0f)));
		for (auto& ai : ghost["AI"])
		{
			enemy->addBehavior(ai.at(0), ai.at(1), ai.at(2));
		}
		nSceneLayers[Enemies]->attachChild(std::move(enemy));
	}

	for (auto& chicken : enemyConfig["Chicken"])
	{
		std::unique_ptr<Chicken> enemy(new Chicken(Enemy::Chicken, toVector2<float>(chicken["Position"]) + sf::Vector2f(16.0f, 16.0f)));
		for (auto& ai : chicken["AI"])
		{
			enemy->addBehavior(ai.at(0), ai.at(1), ai.at(2));
		}
		nSceneLayers[Enemies]->attachChild(std::move(enemy));
	}

	for (auto& snail : enemyConfig["Snail"])
	{
		std::unique_ptr<Snail> enemy(new Snail(Enemy::Snail, toVector2<float>(snail["Position"]) + sf::Vector2f(16.0f, 16.0f)));
		for (auto& ai : snail["AI"])
		{
			enemy->addBehavior(ai.at(0), ai.at(1), ai.at(2));
		}
		nSceneLayers[Enemies]->attachChild(std::move(enemy));
	}
	
	// Dough
	std::unique_ptr<Dough> leader(new Dough(Dough::Dough2));
	nPlayerDough = leader.get();
	nPlayerDough->setPosition(nSpawnPosition);
	nPlayerDough->setCheckPoint(nSpawnPosition);
	nSceneLayers[Player]->attachChild(std::move(leader));

	// Slide

	for (auto& slide : config["SlideBlock"])
	{
		sf::Vector2f position = toVector2<float>(slide["Position"]);
		position.x *= 32; position.y *= 32;
		position.y += 16;
		std::unique_ptr<SlideBlock> slideBlock(new SlideBlock(Block::SlideBlock, position));

		for (auto& pa : slide["Offset"])
		{
			slideBlock->addPath(toVector2<float>(pa));
		}

		nSceneLayers[Map]->attachChild(std::move(slideBlock));
	}

	// Pipes
	for (auto& pipe : config["Pipes"])
	{
		sf::Vector2f position = sf::Vector2f(pipe[0], pipe[1]);
		int height = pipe[2];
		int invert = pipe[3];
		std::unique_ptr<SewerPipe> sewerPipe(new SewerPipe(Block::SewerPipe, position, height, invert));
		nSceneLayers[Pipes]->attachChild(std::move(sewerPipe));
	}
	//Pips Connection
	for (auto& con: config["PipeConnect"])
	{
		int index1 = con[0];
		int index2 = con[1];
		SewerPipe* pipe1 = static_cast<SewerPipe*>(nSceneLayers[Pipes]->getChildren()[index1].get());
		SewerPipe* pipe2 = static_cast<SewerPipe*>(nSceneLayers[Pipes]->getChildren()[index2].get());
		pipe1->setOutPipe(pipe2);
	}

}

bool World::isWin() {
	if (nCup!= nullptr && !nCup->isCheck()) return false;
	if (nPlayerDough == nullptr) return false;
	assert(nPlayerDough != nullptr);
	std::ofstream file("file/Score/score.txt");
	// file << nTime.asSeconds() << std::endl;
	file << nTime.asSeconds() << std::endl;
	// file << nPlayerDough->getHitPoints() << std::endl;
	file << nPlayerDough->getHitPoints() << std::endl;
	// file << nPlayerDough->getScore() << std::endl;
	file << nPlayerDough->getScore() << std::endl;
	file << 1 << std::endl; // win game
	file << (nPlayerDough->getCoinsCount() > 100) << std::endl; // get all coins
	file.close();
	return true;
}

bool World::isLose() {
	return nTime <= sf::Time::Zero || nPlayerDough->isMarkedForRemoval();
}

void World::applyNormal()
{
	Command applyNormal;
	applyNormal.category = Category::Block;
	applyNormal.action = derivedAction<Block>([this] (Block& block, sf::Time)
	{
		block.applyNormal(*nSceneLayers[Player]);
		block.applyNormal(*nSceneLayers[Enemies]);
	});
	
	nCommandQueue.push(applyNormal);
}

void World::handleCollisions()
{
	Command handleCollisions;
	handleCollisions.category = Category::PlayerDough;
	handleCollisions.action = derivedAction<Dough>([this] (Dough& player, sf::Time)
	{
		player.handleCollisionEnemies(*nSceneLayers[Enemies]);
		player.handleCollisionItems(*nSceneLayers[Items]);
	});
	
	nCommandQueue.push(handleCollisions);
}

int World::getGravity()
{
	return nGravity;
}

void World::removeSceneNode()
{
	Command removeSceneNode;
	removeSceneNode.category = Category::ALL;
	std::vector<SceneNode*> nodes;
	removeSceneNode.action = derivedAction<SceneNode>([this, &nodes] (SceneNode& node, sf::Time)
	{
		if ((node.getCategory() & Category::PlayerDough) && !node.isMarkedForRemoval() && !nWorldBounds.intersects(node.getBoundingRect()))
		{
			Dough* nPlayerDough = static_cast<Dough*>(&node);
			nPlayerDough->getDamage(1);
			nPlayerDough->resetCheckPoint();
		}

		if (node.isMarkedForRemoval())
		{	
			if ((node.getCategory() & Category::Entity))
			{
				// sf::FloatRect bound = 
				if (!nWorldBounds.intersects(node.getBoundingRect()))
					nodes.push_back(&node);
			}
			else
			nodes.push_back(&node);
		}
	});
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		nSceneLayers[i]->onCommand(removeSceneNode, sf::Time::Zero);
	}
	for (SceneNode* node : nodes)
	{
		node->remove();
	}
}

void World::save(std::ofstream& saveFile)
{
	/*
		Format:
		- WorlBound
		- SpawnPosition
		- Time
		- Player
		- number of enemies
		- Enemies
		- number of items
		- Items
		- number of blocks
		- Blocks
	*/

	saveFile.write(reinterpret_cast<char*>(&nWorldBounds), sizeof(nWorldBounds));
	saveFile.write(reinterpret_cast<char*>(&nSpawnPosition), sizeof(nSpawnPosition));
	saveFile.write(reinterpret_cast<char*>(&nTime), sizeof(nTime));
	nPlayerDough->save(saveFile);

	//Enemies
	int enemies = nSceneLayers[Enemies]->getChildren().size();
	saveFile.write(reinterpret_cast<char*>(&enemies), sizeof(enemies));
	for (SceneNode::Ptr& enemy : nSceneLayers[Enemies]->getChildren())
	{
		enemy->save(saveFile);
	}

	//Items
	int items = nSceneLayers[Items]->getChildren().size();
	saveFile.write(reinterpret_cast<char*>(&items), sizeof(items));
	for (SceneNode::Ptr& item : nSceneLayers[Items]->getChildren())
	{
		item->save(saveFile);
	}

	//Blocks
	int blocks = nSceneLayers[Map]->getChildren().size();
	saveFile.write(reinterpret_cast<char*>(&blocks), sizeof(blocks));
	for (SceneNode::Ptr& block : nSceneLayers[Map]->getChildren())
	{
		block->save(saveFile);
	}

	//Checkpoints
	int checkPoints = nSceneLayers[Checkpoints]->getChildren().size();
	saveFile.write(reinterpret_cast<char*>(&checkPoints), sizeof(checkPoints));
	for (SceneNode::Ptr& checkPoint : nSceneLayers[Checkpoints]->getChildren())
	{
		checkPoint->save(saveFile);
	}

	//Pipe
	int pipes = nSceneLayers[Pipes]->getChildren().size();
	saveFile.write(reinterpret_cast<char*>(&pipes), sizeof(pipes));
	for (SceneNode::Ptr& pipe : nSceneLayers[Pipes]->getChildren())
	{
		pipe->save(saveFile);
	}
}

void World::load(std::ifstream& saveFile, int lev)
{
	saveFile.read(reinterpret_cast<char*>(&nWorldBounds), sizeof(nWorldBounds));
	sf::Texture& texture = TextureHolder::getInstance().get(static_cast<Textures::ID>(lev));
	sf::IntRect textureRect(nWorldBounds);
	texture.setRepeated(true);
	std::unique_ptr<SpriteNode> background(new SpriteNode(texture, textureRect));
	background->setPosition(nWorldBounds.left, nWorldBounds.top);
	nSceneLayers[Background]->attachChild(std::move(background));

	saveFile.read(reinterpret_cast<char*>(&nSpawnPosition), sizeof(nSpawnPosition));
	saveFile.read(reinterpret_cast<char*>(&nTime), sizeof(nTime));
	
	int type;
	saveFile.read(reinterpret_cast<char*>(&type), sizeof(type));
	std::unique_ptr<Dough> leader(new Dough(static_cast<Dough::Type>(type)));
	nPlayerDough = leader.get();
	nPlayerDough->load(saveFile);
	nSceneLayers[Player]->attachChild(std::move(leader));
	
	int enemies;
	
	saveFile.read(reinterpret_cast<char*>(&enemies), sizeof(enemies));
	for (int i = 0; i < enemies; ++i)
	{
		int type;
		saveFile.read(reinterpret_cast<char*>(&type), sizeof(type));
		if(type == -1) continue;
		sf::Vector2f position;
		saveFile.read(reinterpret_cast<char*>(&position), sizeof(position));

		std::unique_ptr<Enemy> enemy = EnemyFactory::createEnemy(static_cast<Enemy::Type>(type), position);

		enemy->load(saveFile);

		nSceneLayers[Enemies]->attachChild(std::move(enemy));
	}


	int items;
	saveFile.read(reinterpret_cast<char*>(&items), sizeof(items));
	for (int i = 0; i < items; ++i)
	{
		int type;
		saveFile.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (type == -1) continue;
		sf::Vector2f position;
		saveFile.read(reinterpret_cast<char*>(&position), sizeof(position));

		std::unique_ptr<Item> item = ItemFactory::createItem(static_cast<Item::Type>(type), position);
		item->load(saveFile);
		nSceneLayers[Items]->attachChild(std::move(item));
	}

	int blocks;
	saveFile.read(reinterpret_cast<char*>(&blocks), sizeof(blocks));
	for (int i = 0; i < blocks; ++i)
	{
		int type;
		saveFile.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (type == -1) continue;
		sf::Vector2f position;
		saveFile.read(reinterpret_cast<char*>(&position), sizeof(position));
		std::unique_ptr<Block> block = BlockFactory::createBlock(static_cast<Block::Type>(type), position);
		block->load(saveFile);
		nSceneLayers[Map]->attachChild(std::move(block));
	}
	// return;

	int checkPoints;
	saveFile.read(reinterpret_cast<char*>(&checkPoints), sizeof(checkPoints));
	for (int i = 0; i < checkPoints; ++i)
	{
		int type;
		// std::cout << "checkpoint" << std::endl;
		saveFile.read(reinterpret_cast<char*>(&type), sizeof(type));
		sf::Vector2f position;
		saveFile.read(reinterpret_cast<char*>(&position), sizeof(position));
		std::unique_ptr<CheckPoint> checkPoint = std::make_unique<CheckPoint>(static_cast<CheckPoint::Type>(type), position);
		checkPoint->load(saveFile);
		if (type == CheckPoint::End)
		{
			nCup = checkPoint.get();
		}
		nSceneLayers[Checkpoints]->attachChild(std::move(checkPoint));
	}

	//Pipe
	int pipes;
	saveFile.read(reinterpret_cast<char*>(&pipes), sizeof(pipes));
	for (int i = 0; i < pipes; ++i)
	{
		int type;
		saveFile.read(reinterpret_cast<char*>(&type), sizeof(type));
		sf::Vector2f position;
		saveFile.read(reinterpret_cast<char*>(&position), sizeof(position));
		int height;
		saveFile.read(reinterpret_cast<char*>(&height), sizeof(height));
		bool invert;
		saveFile.read(reinterpret_cast<char*>(&invert), sizeof(invert));
		std::unique_ptr<SewerPipe> pipe = std::make_unique<SewerPipe>(static_cast<Block::Type>(type), position, height, invert);
		pipe->load(saveFile);
		nSceneLayers[Pipes]->attachChild(std::move(pipe));
	}
	
	std::string key = "Map/Level" + std::to_string(lev + 1);
	nlohmann::json config = ConfigLoader::getInstance().getConfig(key.c_str());
	for (auto& pipe : config["PipeConnect"])
	{
		int index1 = pipe[0];
		int index2 = pipe[1];
		SewerPipe* pipe1 = static_cast<SewerPipe*>(nSceneLayers[Pipes]->getChildren()[index1].get());
		SewerPipe* pipe2 = static_cast<SewerPipe*>(nSceneLayers[Pipes]->getChildren()[index2].get());
		pipe1->setOutPipe(pipe2);
	}
}