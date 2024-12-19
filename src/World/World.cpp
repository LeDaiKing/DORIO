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
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include "../ConfigLoader.hpp"
#include "BlockFactory.hpp"
#include "nlohmann/json.hpp"

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
	nCategoryLayers[Enemies] |= Category::Enemy;
	nCategoryLayers[Items] |= Category::Item;
	nCategoryLayers[Player] |= Category::PlayerDough;
	loadMap();


	// Add player's Dough
	std::unique_ptr<Dough> leader(new Dough(Dough::Dough1));
	nPlayerDough = leader.get();
	nPlayerDough->setPosition(nSpawnPosition);
	nSceneLayers[Player]->attachChild(std::move(leader));
	
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
}

void World::loadMap()
{
	
	std::string level;
	std::ifstream file("file/Map/map.txt");
	file >> level;
	file.close();
	// std::cout << level << std::endl;

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
	for (int x = 0; x < 3000; x += 32)
	for (int y = 0; y < map.getSize().y; y += 32)
	{
		sf::Color color = map.getPixel(x + 5, y + 5);
		if (color.toInteger() == 0xFF)
		{
			std::unique_ptr<Block> block = BlockFactory::createBlock("Floor" + level, sf::Vector2f(x + 16, y + 16));
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
	}
	

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
		if (node.isMarkedForRemoval())
		{
			if ((node.getCategory()  & Category::Entity))
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