#pragma once
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "../Holder/ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <iostream>


class StateStack: public sf::NonCopyable
{
    public:
        enum Action
        {
            Push,
            Pop,
            Clear,
        };
    public:
        explicit StateStack(State::Context context);

        template <typename T>
        void registerState(States::ID stateID);

        void update(sf::Time dt);
        void draw();
        void handleEvent(const sf::Event& event);

        void pushState(States::ID stateID);
        void popState();
        void clearStates();

        bool isEmpty() const;
    private:
        State::Ptr createState(States::ID stateID);
        void applyPendingChanges();
    private:
        struct PendingChange
        {
            explicit PendingChange(Action action, States::ID stateID = States::ID::None) {
                this->action = action;
                this->stateID = stateID;
            }
            Action action;
            States::ID stateID;
        };
    private:
        std::vector<State::Ptr> nStack;
        std::vector<PendingChange> nPendingList;
        State::Context nContext;
        std::map<States::ID, std::function<State::Ptr()>> nFactories;
};




template <typename T>
void StateStack::registerState(States::ID stateID) {
    std::cerr << "Registering state " << stateID << std::endl;
    nFactories[stateID] = [this]() {
        return State::Ptr(new T(*this, nContext));
    };
}
