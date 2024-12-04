#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include "BreakableBlock.hpp"
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
, nSpawnPosition(300, nWorldBounds.height - nWorldView.getSize().y / 2.f)
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
	removeEntities();

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

	loadMap();


	// Add player's Dough
	std::unique_ptr<Dough> leader(new Dough(Dough::Dough1));
	nPlayerDough = leader.get();
	nPlayerDough->setPosition(nSpawnPosition);
	nSceneLayers[Player]->attachChild(std::move(leader));
	nCategoryLayers[Player] |= Category::PlayerDough;
	
	nCategoryLayers[Enemies] |= Category::Enemy;
	std::unique_ptr<Enemy> leader1(new Enemy(nSpawnPosition + sf::Vector2f(100, 0)));
	nSceneLayers[Enemies]->attachChild(std::move(leader1));
	leader1.reset(new Enemy(nSpawnPosition + sf::Vector2f(200, 0)));
	nSceneLayers[Enemies]->attachChild(std::move(leader1));
	leader1.reset(new Enemy(nSpawnPosition + sf::Vector2f(300, 0)));
	nSceneLayers[Enemies]->attachChild(std::move(leader1));
	leader1.reset(new Enemy(nSpawnPosition + sf::Vector2f(400, 0)));
	nSceneLayers[Enemies]->attachChild(std::move(leader1));
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

	if (postiion.x > nWorldView.getCenter().x)
	{
		nWorldView.move(postiion.x - nWorldView.getCenter().x, 0);
	}
	else if (postiion.x < nWorldView.getCenter().x - nWorldView.getSize().x / 5.f)
	{
		nWorldView.move(postiion.x - nWorldView.getCenter().x + nWorldView.getSize().x / 5.f, 0);
	}

	// nWorldView.move(0.0001, 0);
}

void World::loadMap()
{
	nCategoryLayers[Map] |= Category::Block;

	// Prepare the tiled background
	sf::Texture& texture = TextureHolder::getInstance().get(Textures::Sky);
	sf::IntRect textureRect(nWorldBounds);
	texture.setRepeated(true);
	sf::Image map; map.loadFromFile("res/Background/map1_3.png");
	for (int x = 0; x < map.getSize().x; x += 32)
	for (int y = 0; y < map.getSize().y; y += 32)
	{
		sf::Color color = map.getPixel(x + 5,y + 5);
		// std::cout << color.toInteger() << std::endl;s
		if (color == sf::Color::Black || color == sf::Color::Blue)
		{
			// std::cout << x << " " << y << std::endl;
			std::unique_ptr<Block> block(new Block(Block::Dirt, sf::Vector2f(x, y)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
		else if (color == sf::Color::Red)
		{
			std::unique_ptr<Block> block(new BreakableBlock(sf::Vector2f(x, y)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}

	}

	// // Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(nWorldBounds.left, nWorldBounds.top);
	nSceneLayers[Background]->attachChild(std::move(backgroundSprite));
	nCategoryLayers[Background] |= Category::Scene;
	return;


	// Add the dirt sprite to the scene
	for (int i = 0; i < 60; i++)
	{
		std::unique_ptr<Block> block(new Block(Block::Dirt, sf::Vector2f(32 * i, nSpawnPosition.y + 96)));
		nSceneLayers[Map]->attachChild(std::move(block));
		block.reset(new Block(Block::Dirt, sf::Vector2f(32 * i, nSpawnPosition.y + 128)));
		nSceneLayers[Map]->attachChild(std::move(block));

		if (i == 8)
		{
			std::unique_ptr<Block> block(new Block(Block::Dirt, sf::Vector2f(48 * i, nSpawnPosition.y + 48)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}

		if (i == 5)
		{
			std::unique_ptr<Block> block(new Block(Block::Dirt, sf::Vector2f(48 * i, nSpawnPosition.y + -50)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}

		if (i == 10)
		{
			std::unique_ptr<Block> block(new Block(Block::Dirt, sf::Vector2f(48 * i, nSpawnPosition.y)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
	}

	std::unique_ptr<Block> breakable(new BreakableBlock(sf::Vector2f(32 * 20, nSpawnPosition.y)));
	nSceneLayers[Map]->attachChild(std::move(breakable));
	breakable.reset(new BreakableBlock(sf::Vector2f(32 * 21, nSpawnPosition.y)));
	nSceneLayers[Map]->attachChild(std::move(breakable));
	breakable.reset(new BreakableBlock(sf::Vector2f(32 * 22, nSpawnPosition.y)));
	nSceneLayers[Map]->attachChild(std::move(breakable));
	breakable.reset(new BreakableBlock(sf::Vector2f(32 * 23, nSpawnPosition.y + 32)));
	nSceneLayers[Map]->attachChild(std::move(breakable));

	

	nCategoryLayers[Map] |= Category::Block;
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
	});
	
	nCommandQueue.push(handleCollisions);
}

int World::getGravity()
{
	return nGravity;
}

void World::removeEntities()
{
	std::vector<SceneNode::Ptr>& children = nSceneLayers[Enemies]->getChildren();
	std::vector<Enemy*> enemies;
	for (SceneNode::Ptr& child : children)
	{
		Enemy& enemy = static_cast<Enemy&>(*child);
		if (enemy.isDead())
		{
			enemies.push_back(&enemy);
		}
	}

	for (Enemy* enemy : enemies)
	{
		enemy->remove();
	}
}