#include "ParallelTask.hpp"
#include <SFML/System/Lock.hpp>

bool ParallelTask::isFinished(){
    sf::Lock lock(nMutex);
    return nFinished;
}

void ParallelTask::runTask() {
    bool ended = false;
    while (!ended) {
        sf::Lock lock(nMutex);
        if (nElapsedTime.getElapsedTime().asSeconds() >= 10.f) {
            ended = true;
        }
    }

    {
        sf::Lock lock(nMutex);
        nFinished = true;
    }
}