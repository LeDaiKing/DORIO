#pragma once

#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Block.hpp"
#include "Dough.hpp"
#include "../Command/CommandQueue.hpp"
#include "../Command/Command.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>



namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	public:
		explicit World(sf::RenderWindow& window);
		void update(sf::Time dt);
		void draw();
		
		CommandQueue& getCommandQueue();


	private:
		void loadTextures();
		void buildScene();
		void adaptPlayerPosition();
		void applyGravity();
		void applyNormal();
		void adaptCameraPosition();
		void loadMap();


	private:
		enum Layer
		{
			Background,
			Map,
			Air,
			LayerCount
		};


	private:
		sf::RenderWindow& nWindow;
		sf::View nWorldView;
		TextureHolder nTextures;

		SceneNode nSceneGraph;
		std::array<SceneNode*, LayerCount>	nSceneLayers;
		unsigned int nCategoryLayers[LayerCount];
		CommandQueue nCommandQueue;

		sf::FloatRect nWorldBounds;
		sf::Vector2f nSpawnPosition;
		// float								mScrollSpeed;
		Dough* nPlayerDough;

		int nGravity;
};
