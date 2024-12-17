#include "LoadingState.hpp"
#include <iostream> 

LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context)
{
    nLoadingText.setFont(FontHolder::getInstance().get(Fonts::Main));
    nLoadingText.setString("Loading Resources");
    centerOrigin(nLoadingText);
    nLoadingText.setPosition(context.window->getView().getSize() / 2.f);

    nProgressBarBackground.setFillColor(sf::Color::White);
    nProgressBarBackground.setSize(sf::Vector2f(context.window->getView().getSize().x - 20, 10));
    nProgressBarBackground.setPosition(10, nLoadingText.getPosition().y + 40);

    nProgressBar.setFillColor(sf::Color(100, 100, 100));
    nProgressBar.setSize(sf::Vector2f(200, 10));
    nProgressBar.setPosition(10, nLoadingText.getPosition().y + 40);

    context.loadingTask->execute();
}

void LoadingState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.clear(sf::Color::Black);
    window.draw(nLoadingText);
    window.draw(nProgressBarBackground);
    window.draw(nProgressBar);
}

bool LoadingState::update(sf::Time dt) {
    ParallelTask& loadingTask = *getContext().loadingTask;
    if (loadingTask.isFinished()) {
        requestStackPop();
        // requestStackPush(States::Game);
    } else {
        float completion = loadingTask.getCompletion();
        setCompletion(completion);
    }
    return false;
}

bool LoadingState::handleEvent(const sf::Event& event) {
    return false;
}

void LoadingState::setCompletion(float percent) {
    nProgressBar.setSize(sf::Vector2f(nProgressBarBackground.getSize().x * percent, nProgressBar.getSize().y));
}
