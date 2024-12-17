#include "ParallelTask.hpp"
#include <SFML/System/Lock.hpp>
#include <iostream>

const float timePerTask = 4.0f;

ParallelTask::ParallelTask()
: nThread(&ParallelTask::runTask, this)
, nFinished(false)
{

}

void ParallelTask::execute() {
  nFinished = false;
  nElapsedTime.restart();
  nThread.launch();
}

bool ParallelTask::isFinished() {
  sf::Lock lock(nMutex);
  return nFinished;
}

float ParallelTask::getCompletion() {
  sf::Lock lock(nMutex);
  std::cerr << "Elapsed time: " << nElapsedTime.getElapsedTime().asSeconds() / timePerTask << std::endl;
  return nElapsedTime.getElapsedTime().asSeconds() / timePerTask;
}

void ParallelTask::runTask() {
    bool ended = false;
    while (!ended) {
        sf::Lock lock(nMutex);
        if (nElapsedTime.getElapsedTime().asSeconds() >= timePerTask) {
            ended = true;
        }
    }

    {
        sf::Lock lock(nMutex);
        nFinished = true;
    }
}