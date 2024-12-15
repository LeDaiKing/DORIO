#include "StateStack.hpp"
#include <cassert>
#include <iostream>


State::Ptr StateStack::createState(States::ID stateID) {
    auto found = nFactories.find(stateID);
    std::cerr << stateID << std::endl;
    assert(found != nFactories.end());

    return found->second();
}

void StateStack::applyPendingChanges() {
    for (PendingChange change : nPendingList) {
        switch (change.action) {
            case Action::Push:
                nStack.push_back(createState(change.stateID));
                break;
            case Action::Pop:
                nStack.pop_back();
                break;
            case Action::Clear:
                nStack.clear();
                break;
        }
    }

    nPendingList.clear();
}

StateStack::StateStack(State::Context context)
: nStack()
, nPendingList()
, nContext(context)
, nFactories() {
}

void StateStack::update(sf::Time dt) {
  for (auto itr = nStack.rbegin(); itr != nStack.rend(); ++itr) {
    if (!(*itr)->update(dt)) {
      break;
    }
  }

  applyPendingChanges();
}

void StateStack::draw() {
    for (State::Ptr& state : nStack) {
        state->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event) {
  for (auto itr = nStack.rbegin(); itr != nStack.rend(); ++itr) {
    if (!(*itr)->handleEvent(event)) {
      break;
    }
  }

  applyPendingChanges();
}

void StateStack::pushState(States::ID stateID) {
    nPendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState() {
    nPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates() {
    nPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const {
    return nStack.empty();
}
