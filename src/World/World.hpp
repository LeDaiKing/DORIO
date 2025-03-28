#pragma once

#include "../Holder/ResourceHolder.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Block.hpp"
#include "Dough.hpp"
#include "Enemy.hpp"
#include "Item.hpp"
#include "Hub.hpp"
#include "CheckPoint.hpp"
#include "../Command/CommandQueue.hpp"
#include "../Command/Command.hpp"
#include "../StateStack/State.hpp"

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
		explicit World(sf::RenderWindow& window, State::Context context);
		void update(sf::Time dt);
		void draw();
		
		CommandQueue& getCommandQueue();

		static int getGravity();
		void save(std::ofstream& saveFile);
		void load(std::ifstream& saveFile, int lev);
		void loadMap(std::string level);
		bool isWin();
		bool isLose();


	private:
		// void loadTextures();
		void buildScene();
		void applyGravity();
		void applyNormal();
		void adaptCameraPosition();

		void handleCollisions();
		void removeSceneNode();

	private:
		enum Layer
		{
			Background,
			Map,
			Checkpoints,
			Enemies,
			Items,
			Player,
			Pipes,
			LayerCount
		};


	private:
		sf::RenderWindow& nWindow;
		sf::View nWorldView;
		State::Context nContext;

		SceneNode nSceneGraph;
		std::array<SceneNode*, LayerCount>	nSceneLayers;
		unsigned int nCategoryLayers[LayerCount];
		CommandQueue nCommandQueue;

		sf::FloatRect nWorldBounds;
		sf::Vector2f nSpawnPosition;
		// float								mScrollSpeed;
		Dough* nPlayerDough;
		Dough* nPlayerDough2;
		CheckPoint* nCup;
		
		Hub nHub;
		sf::Time nTime;

		static int nGravity;
		int nLevel;
};
