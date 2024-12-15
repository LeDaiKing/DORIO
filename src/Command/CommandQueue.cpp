
#include "CommandQueue.hpp"
// #include "../World/SceneNode.hpp"


void CommandQueue::push(const Command& command)
{
	nQueue.push(command);
}

Command CommandQueue::pop()
{
	Command command = nQueue.front();
	nQueue.pop();
	return command;
}

bool CommandQueue::isEmpty() const
{
	return nQueue.empty();
}