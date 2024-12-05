#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include "BreakableBlock.hpp"
#include "Coin.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

int World::nGravity = 512;

World::World(sf::RenderWindow& window)
: nWindow(window)
, nWorldView(window.getDefaultView())
, nSceneGraph()
, nSceneLayers()
, nWorldBounds(0.f, 0.f, 5000, nWorldView.getSize().y)
, nSpawnPosition(50, nWorldBounds.height - nWorldView.getSize().y / 2.f)
, nPlayerDough(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	//zoom(0.5f);
	nWorldView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	// nWorldBounds = sf::FloatRect(0.f, 0.f, 10, 10000.f);
	nWorldView.setCenter(nWorldView.getSize().x / 4.f, nWorldView.getSize().y / 2.f);
	nWorldView.zoom(0.5f);
}

void World::update(sf::Time dt)
{
	
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

	nSceneGraph.update(dt);
	// Regular update step, adapt position (correct if outside view)

	adaptCameraPosition();
}

void World::draw()
{
	nWindow.setView(nWorldView);
	nWindow.draw(nSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return nCommandQueue;
}

void World::loadTextures()
{
	TextureHolder::getInstance().load(Textures::Dirt, "res/Background/Dirt.png");
	TextureHolder::getInstance().load(Textures::Dough1, "res/Dough/dough.png");
	TextureHolder::getInstance().load(Textures::Dough2, "res/Dough/tile001.png");
	TextureHolder::getInstance().load(Textures::Sky, "res/Background/Blue.png");
	TextureHolder::getInstance().load(Textures::Enemy, "res/Enemy/Enemy.png");
	TextureHolder::getInstance().load(Textures::Breakable, "res/Background/Breakable/Breakable.png");
	TextureHolder::getInstance().load(Textures::BreakAnimation, "res/Background/Breakable/BreakAnimation.png");
	TextureHolder::getInstance().load(Textures::Coin, "res/Item/Coin.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
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
	

	// Prepare the tiled background
	sf::Texture& texture = TextureHolder::getInstance().get(Textures::Sky);
	sf::IntRect textureRect(nWorldBounds);
	texture.setRepeated(true);
	sf::Image map; map.loadFromFile("res/Background/map.png");
	nWorldBounds.width = map.getSize().x;
	for (int x = 0; x < map.getSize().x; x += 32)
	for (int y = 0; y < map.getSize().y; y += 32)
	{
		sf::Color color = map.getPixel(x + 5,y + 5);
		// std::cout << color.toInteger() << std::endl;
		if (color.toInteger() == 0x000000 + 255)
		{
			// std::cout << x << " " << y << std::endl;
			std::unique_ptr<Block> block(new Block(Block::Dirt, sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xFF000000 + 255)
		{
			std::unique_ptr<Block> block(new BreakableBlock(sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0x0DFF0000 + 255)
		{
			std::unique_ptr<Enemy> leader1(new Enemy(sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Enemies]->attachChild(std::move(leader1));
		}
		else if (color.toInteger() == 0xFFFC0000 + 255)
		{
			std::unique_ptr<Coin> coin(new Coin(sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Items]->attachChild(std::move(coin));
		}

	}
	// std::unique_ptr<Block> block(new BreakableBlock(sf::Vector2f(4 * 32 + 16, 14 * 32 + 16)));
	// nSceneLayers[Map]->attachChild(std::move(block));

	// // Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(nWorldBounds.left, nWorldBounds.top);
	nSceneLayers[Background]->attachChild(std::move(backgroundSprite));

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