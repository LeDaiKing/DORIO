#include "ChoosePlayerState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"

ChoosePlayerState::ChoosePlayerState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, GUI::Button::Type::BackButton)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    const int START = 75;
    auto slotButton1 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SquareButton);
    slotButton1->setPosition(START + 150, 300);
    slotButton1->setText("1 player");
    slotButton1->setColor(sf::Color::Red);
    slotButton1->setSizeText(30);
    slotButton1->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });

    auto slotButton2 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SquareButton);
    slotButton2->setPosition(START + 450 + 20, 300);
    slotButton2->setText("2 players");
    slotButton2->setColor(sf::Color::Red);
    slotButton2->setSizeText(30);
    slotButton2->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseCharacter);
    });


    auto textInstruction = std::make_shared<GUI::Label>("How many people are playing");
    textInstruction->setCharacterSize(30);
    textInstruction->setColor(sf::Color::White);
    textInstruction->setPosition((800 - textInstruction->getWidth()) / 2, 150);  

    backButton.setPosition(62.5f, 600 - 105.0f / 2.0f);
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::ChooseMode);
    });  

    nGUIContainer.pack(slotButton1);
    nGUIContainer.pack(slotButton2);
    nGUIContainer.pack(textInstruction);
}

void ChoosePlayerState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
}

bool ChoosePlayerState::update(sf::Time dt)
{
    return true;
}

bool ChoosePlayerState::handleEvent(const sf::Event& event)
{
    nGUIContainer.handleEvent(event);
    backButton.handleEvent(event);
    return false;
}
