#include "SceneNode.hpp"
#include "../Command/Command.hpp"


#include <algorithm>
#include <cassert>


SceneNode::SceneNode()
: nChildren()
, nParent(nullptr)
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

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt)
{
	for(Ptr& child : nChildren)
		child->update(dt);
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
		child->draw(target, states);
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
		child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
	return Category::Scene;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

std::vector<SceneNode::Ptr>& SceneNode::getChildren() 
{
	return nChildren;
}