#include "World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>


World::World(sf::RenderWindow& window)
: nWindow(window)
, nWorldView(window.getDefaultView())
, nTextures() 
, nSceneGraph()
, nSceneLayers()
, nWorldBounds(0.f, 0.f, nWorldView.getSize().x, 2000.f)
, nSpawnPosition(nWorldView.getSize().x / 2.f, nWorldBounds.height - nWorldView.getSize().y / 2.f)
, nPlayerDough(nullptr)
, nGravity(512)
{
	loadTextures();
	buildScene();

	// Prepare the view
	//zoom(0.5f);
	// nWorldView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	nWorldView.zoom(0.5f);
	nWorldView.setCenter(nSpawnPosition);
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	// nWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	
	// nPlayerDough->setVelocity(0.f, 0.f);
	applyGravity();
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
	// adaptPlayerVelocity();

	nSceneGraph.update(dt);
	// Regular update step, adapt position (correct if outside view)
	adaptPlayerPosition();
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
	nTextures.load(Textures::normal, "res/Dough/tile001.png");
    nTextures.load(Textures::Sky, "res/Background/sky.png");
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

	// Prepare the tiled background
	sf::Texture& texture = nTextures.get(Textures::Sky);
	sf::IntRect textureRect(nWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(nWorldBounds.left, nWorldBounds.top);
	nSceneLayers[Background]->attachChild(std::move(backgroundSprite));
	nCategoryLayers[Background] |= Category::Scene;

	// Add player's Dough
	std::unique_ptr<Dough> leader(new Dough(Dough::normal, nTextures));
	nPlayerDough = leader.get();
	nPlayerDough->setPosition(nSpawnPosition);
	nSceneLayers[Air]->attachChild(std::move(leader));
	nCategoryLayers[Air] |= Category::PlayerDough;
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(nWorldView.getCenter() - nWorldView.getSize() / 2.f, nWorldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = nPlayerDough->getPosition();
	// position.x = std::max(position.x, viewBounds.left + borderDistance);
	// position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	// position.y = std::max(position.y, viewBounds.top + borderDistance);
	// position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	// if (position.y > viewBounds.top + viewBounds.height - borderDistance)
	// {
	// 	nPlayerDough->accelerate(0.f, -nGravity);
	// 	// nPlayerDough->setPosition(position.x, viewBounds.top + viewBounds.height - borderDistance - 16);
	// }
}

void World::applyGravity()
{
	Command applyGravity;
	applyGravity.category = Category::PlayerDough;
	applyGravity.action = derivedAction<Entity>([this] (Entity& Dough, sf::Time)
	{
		sf::FloatRect viewBounds(nWorldView.getCenter() - nWorldView.getSize() / 2.f, nWorldView.getSize());
		const float borderDistance = 40.f;

		

		sf::Vector2f position = Dough.getPosition();
		// position.x = std::max(position.x, viewBounds.left + borderDistance);
		// position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
		// position.y = std::max(position.y, viewBounds.top + borderDistance);
		// position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
		if (position.y  > viewBounds.top + viewBounds.height - borderDistance)
		{
			if (Dough.getVelocity().y > 0)
				Dough.addVelocity(0.f, -Dough.getVelocity().y);
			// nPlayerDough->setPosition(position.x, viewBounds.top + viewBounds.height - borderDistance - 16);
		} else
		Dough.accelerate(0.f, nGravity);
		
	});
	
	nCommandQueue.push(applyGravity);
}