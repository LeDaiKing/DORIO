#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>


World::World(sf::RenderWindow& window)
: nWindow(window)
, nWorldView(window.getDefaultView())
, nSceneGraph()
, nSceneLayers()
, nWorldBounds(0.f, 0.f, 5000, nWorldView.getSize().y)
, nSpawnPosition(50, nWorldBounds.height - nWorldView.getSize().y / 2.f)
, nPlayerDough(nullptr)
, nGravity(512)
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
	enemiesAttackPlayer();
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
	TextureHolder::getInstance().load(Textures::Dough1, "res/Dough/DoughSheet.png");
	TextureHolder::getInstance().load(Textures::Dough2, "res/Dough/tile001.png");
	TextureHolder::getInstance().load(Textures::Sky, "res/Background/Blue.png");
	TextureHolder::getInstance().load(Textures::Enemy, "res/Enemy/Enemy.png");

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
	std::unique_ptr<Dough> leader(new Dough(Dough::Dough2));
	nPlayerDough = leader.get();
	nPlayerDough->setPosition(nSpawnPosition);
	nSceneLayers[Player]->attachChild(std::move(leader));
	nCategoryLayers[Player] |= Category::PlayerDough;
	
	std::unique_ptr<Enemy> leader1(new Enemy(nSpawnPosition + sf::Vector2f(100, 0)));
	nSceneLayers[Enemies]->attachChild(std::move(leader1));
	nCategoryLayers[Enemies] |= Category::Enemy;
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

	if (postiion.x > nWorldView.getCenter().x + nWorldView.getSize().x / 4.f)
	{
		nWorldView.move(postiion.x - nWorldView.getCenter().x - nWorldView.getSize().x / 4.f, 0);
	}
	else if (postiion.x < nWorldView.getCenter().x - nWorldView.getSize().x / 4.f - 50)
	{
		nWorldView.move(postiion.x - nWorldView.getCenter().x + nWorldView.getSize().x / 4.f + 50, 0);
	}

	// nWorldView.move(0.0001, 0);
}

void World::loadMap()
{
	// Prepare the tiled background
	sf::Texture& texture = TextureHolder::getInstance().get(Textures::Sky);
	sf::IntRect textureRect(nWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(nWorldBounds.left, nWorldBounds.top);
	nSceneLayers[Background]->attachChild(std::move(backgroundSprite));
	nCategoryLayers[Background] |= Category::Scene;


	// Add the dirt sprite to the scene
	for (int i = 0; i < 60; i++)
	{
		std::unique_ptr<Block> block(new Block(sf::Vector2f(48 * i, nSpawnPosition.y + 128)));
		nSceneLayers[Map]->attachChild(std::move(block));

		if (i == 8)
		{
			std::unique_ptr<Block> block(new Block(sf::Vector2f(48 * i, nSpawnPosition.y + 80)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}

		if (i == 5)
		{
			std::unique_ptr<Block> block(new Block(sf::Vector2f(48 * i, nSpawnPosition.y + -18)));
			nSceneLayers[Map]->attachChild(std::move(block));
		}
	}
	

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

void World::enemiesAttackPlayer()
{
	Command attackPlayer;
	attackPlayer.category = Category::Enemy;
	attackPlayer.action = derivedAction<Enemy>([this] (Enemy& enemy, sf::Time)
	{
		enemy.attackPlayer(*nPlayerDough);
	});
	
	nCommandQueue.push(attackPlayer);
}