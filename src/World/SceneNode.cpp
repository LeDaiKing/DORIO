#include "SceneNode.hpp"
#include "../Command/Command.hpp"
#include "Utility.hpp"

#include <algorithm>
#include <cassert>

sf::Vector2f SceneNode::nRange = sf::Vector2f(0, 0);

SceneNode::SceneNode(unsigned int category)
: nChildren()
, nParent(nullptr)
, nDefaultCategory(category)
{
}


void SceneNode::attachChild(Ptr child)
{
	child->nParent = this;
	nChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(nChildren.begin(), nChildren.end(), [&] (Ptr& p) { return p.get() == &node; });
	assert(found != nChildren.end());

	Ptr result = std::move(*found);
	result->nParent = nullptr;
	nChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for(Ptr& child : nChildren)
	{
		if (!checkInRange(child->getBoundingRect(), nRange)) continue;
		child->update(dt, commands);
	}
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Ptr& child : nChildren)
	{
		if (!checkInRange(child->getBoundingRect(), nRange)) continue;
		child->draw(target, states);
	}
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->nParent)
		transform = node->getTransform() * transform;

	return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// Command current node, if category matches
	
	if (command.category & getCategory())
		command.action(*this, dt);
	// else if (command.category != Category::Scene)
	// 	return;

	// Command children
	for(Ptr& child : nChildren)
	{
		if (!checkInRange(child->getBoundingRect(), nRange)) continue;
		child->onCommand(command, dt);
	}
}

unsigned int SceneNode::getCategory() const
{
	return nDefaultCategory;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

std::vector<SceneNode::Ptr>& SceneNode::getChildren() 
{
	return nChildren;
}

void SceneNode::remove()
{
	nParent->detachChild(*this);
}

bool SceneNode::isMarkedForRemoval() const
{
	return false;
}

void SceneNode::save(std::ofstream& file)
{
	//somthing
}

void SceneNode::load(std::ifstream& file)
{
	//somthing
}

void SceneNode::setRange(float centerPoint)
{
	nRange = sf::Vector2f(centerPoint - 700, centerPoint + 700);
}