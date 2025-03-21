#pragma once

#include "Category.hpp"
#include "../Command/CommandQueue.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <fstream>
#include <vector>
#include <memory>


struct Command;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;


	public:
		// SceneNode();
		SceneNode(unsigned int category = Category::Scene);

		void attachChild(Ptr child);
		Ptr	detachChild(const SceneNode& node);
		
		void update(sf::Time dt, CommandQueue& commands);

		sf::Vector2f getWorldPosition() const;
		sf::Transform getWorldTransform() const;

		void onCommand(const Command& command, sf::Time dt);
		virtual unsigned int getCategory() const;

		virtual sf::FloatRect getBoundingRect() const;

		virtual bool isMarkedForRemoval() const;
		void remove();

		virtual void save(std::ofstream& file);
		virtual void load(std::ifstream& file);

		std::vector<Ptr>& getChildren();

		static void setRange(float centerPoint);

	protected:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		void updateChildren(sf::Time dt, CommandQueue& commands);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		std::vector<Ptr> nChildren;
		static sf::Vector2f nRange;
	protected:
		SceneNode* nParent;
		unsigned int nDefaultCategory;
};
