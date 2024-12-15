#pragma once
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Mutex.hpp>


class ParallelTask {
    public:
        ParallelTask();
        void execute();
        bool isFinished();
        float getCompletion();
    private:
        void runTask();
    private:
        sf::Thread nThread;
        bool nFinished;
        sf::Clock nElapsedTime;
        sf::Mutex nMutex;
};