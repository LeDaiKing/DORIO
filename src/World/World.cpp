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

int World::nGravity = 700;

World::World(sf::RenderWindow& window)
: nWindow(window)
, nWorldView(window.getDefaultView())
, nSceneGraph()
, nSceneLayers()
, nWorldBounds(0.f, 0.f, 5000, nWorldView.getSize().y)
, nSpawnPosition(50, nWorldBounds.height - nWorldView.getSize().y / 2.f)
, nPlayerDough(nullptr)
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
	nHub.update(dt, *nPlayerDough);
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
	std::unique_ptr<Dough> leader(new Dough(Dough::Dough2));
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
	sf::Image map; map.loadFromFile("res/Background/map2.png");
	nWorldBounds.width = map.getSize().x;
	for (int x = 0; x < map.getSize().x; x += 32)
	for (int y = 0; y < map.getSize().y; y += 32)
	{
		sf::Color color = map.getPixel(x + 5,y + 5);
		// std::cout << color.toInteger() << std::endl;
		if (color.toInteger() == 0x000000 + 255 || (x == 0 && y == 16 * 32))
		{
			// std::cout << x << " " << y << std::endl;
			std::unique_ptr<Block> block(new StaticBlock(StaticBlock::Dirt, sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0xFF000000 + 255)
		{
			std::unique_ptr<Block> block(new BreakableBlock(BreakableBlock::Breakable, sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color.toInteger() == 0x0DFF0000 + 255)
		{
			std::unique_ptr<Enemy> leader1(new CockRoach(Enemy::CockRoach, sf::Vector2f(x + 16, y + 16)));
		
			if (x == 320)
			{
				leader1->addWaitBehavior(sf::seconds(1));
				leader1->addMoveBehavior(sf::Vector2f(32 * 4, 0));
				leader1->addWaitBehavior(sf::seconds(2));
				leader1->addTurnBehavior();

				leader1->addWaitBehavior(sf::seconds(2));
				leader1->addMoveBehavior(sf::Vector2f(32  * 2, 0));
				leader1->addWaitBehavior(sf::seconds(2));
				leader1->addTurnBehavior();

				leader1->addWaitBehavior(sf::seconds(5));
				leader1->addMoveBehavior(sf::Vector2f(-32 * 6, 0));
				leader1->addWaitBehavior(sf::seconds(2));
				leader1->addTurnBehavior();

				leader1->addWaitBehavior(sf::seconds(2));
				leader1->addMoveBehavior(sf::Vector2f(-32 * 2, 0));
				leader1->addWaitBehavior(sf::seconds(2));
			}
			nSceneLayers[Enemies]->attachChild(std::move(leader1));
		}
		else if (color.toInteger() == 0xFFFC0000 + 255)
		{
			std::unique_ptr<Coin> coin(new Coin(Item::Coin, sf::Vector2f(x + 16, y + 16)));
			nSceneLayers[Items]->attachChild(std::move(coin));
		}
		else if (color.toInteger() == 0x0000FF00 + 255)
		{
			std::unique_ptr<LuckyBlock> block(new LuckyBlock(Block::LuckyBlock, sf::Vector2f(x + 16, y + 16)));
			block->addItem(Item::FireBig);
			block->addItem(Item::Coin);
			block->addItem(Item::Coin);
			block->addItem(Item::Coin);
			block->addItem(Item::Coin);
			nSceneLayers[Map]->attachChild(std::move(block));
		}

	}
	// std::unique_ptr<Block> block(new BreakableBlock(sf::Vector2f(4 * 32 + 16, 14 * 32 + 16)));
	// nSceneLayers[Map]->attachChild(std::move(block));
	std::unique_ptr<SlideBlock> block(new SlideBlock(Block::Dirt, sf::Vector2f(22 * 32 + 16, 12 * 32 + 16)));
	block->addPath(sf::Vector2f(0, -32 * 3));
	block->addPath(sf::Vector2f(32 * 5, 0));
	nSceneLayers[Map]->attachChild(std::move(block));

	std::unique_ptr<JumpyBlock> block1(new JumpyBlock(Block::JumpyBlock, sf::Vector2f(32 * 13 + 16, 32 * 14 + 16)));
	nSceneLayers[Map]->attachChild(std::move(block1));

	std::unique_ptr<Ghost> enemy(new Ghost(Enemy::Ghost, sf::Vector2f(32 * 16 + 16, 32 * 7 + 16)));
	enemy->addWaitBehavior(sf::seconds(2));
	enemy->addMoveBehavior(sf::Vector2f(32 * 4, 0));
	enemy->addWaitBehavior(sf::seconds(2));
	enemy->addTurnBehavior();
	enemy->addWaitBehavior(sf::seconds(2));
	enemy->addMoveBehavior(sf::Vector2f(- 32 * 7, 32 * 5));
	enemy->addWaitBehavior(sf::seconds(2));
	enemy->addTurnBehavior();
	enemy->addWaitBehavior(sf::seconds(2));
	enemy->addMoveBehavior(sf::Vector2f(32 * 3, 0));
	enemy->addWaitBehavior(sf::seconds(2));
	enemy->addMoveBehavior(sf::Vector2f(0, -32 * 5));
	enemy->addWaitBehavior(sf::seconds(1));
	nSceneLayers[Enemies]->attachChild(std::move(enemy));

	std::unique_ptr<Chicken> enemy1(new Chicken(Enemy::Chicken, sf::Vector2f(32 * 16 + 16, 32 * 16 + 16)));
	// enemy1->addWaitBehavior(sf::seconds(2));
	nSceneLayers[Enemies]->attachChild(std::move(enemy1));

	std::unique_ptr<Snail> enemy2(new Snail(Enemy::Snail, sf::Vector2f(32 * 5 + 16, 32 * 16 + 16)));
	nSceneLayers[Enemies]->attachChild(std::move(enemy2));

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