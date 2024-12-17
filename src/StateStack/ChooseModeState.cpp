#include "ChooseModeState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"


ChooseModeState::ChooseModeState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainer()
, backButton(context, GUI::Button::Type::BackButton)
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));

    auto slotButton1 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SquareButton);
    const float bonusX = 1.75f;
    const float bonusY = 1.33f;
    slotButton1->setPosition(140 * bonusX, 190 * bonusY);
    slotButton1->setText("NEW!");
    slotButton1->setColor(sf::Color::Red);
    slotButton1->setSizeText(30);
    slotButton1->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });

    auto slotButton2 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SquareButton);
    slotButton2->setPosition((260 + 140) * bonusX, 190 * bonusY);
    slotButton2->setText("NEW!");
    slotButton2->setColor(sf::Color::Red);
    slotButton2->setSizeText(30);
    slotButton2->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });

    auto slotButton3 = std::make_shared<GUI::Button>(context, GUI::Button::Type::SquareButton);
    slotButton3->setPosition((260 + 140 + 260) * bonusX, 190 * bonusY);
    slotButton3->setText("NEW!");
    slotButton3->setColor(sf::Color::Red);
    slotButton3->setSizeText(30);
    slotButton3->setCallback([this] ()
    {

        requestStackPop();
        requestStackPush(States::Game);
    });

    auto creativeButton = std::make_shared<GUI::Button>(context, GUI::Button::Type::CreativeButton);
    creativeButton->setPosition(400 * bonusX, 400 * bonusY);
    creativeButton->setText("Creative");
    creativeButton->setColor(sf::Color::Red);
    creativeButton->setSizeText(30);
    creativeButton->setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });


    backButton.setPosition(62.5f, 800 - 105.0f / 2.0f);
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Title);
    });


    auto textNormalMode = std::make_shared<GUI::Label>("Normal Mode");
    textNormalMode->setCharacterSize(30);
    textNormalMode->setColor(sf::Color::Yellow);
    textNormalMode->setPosition((1400 - textNormalMode->getWidth()) / 2, 50 * bonusY);    

    auto textSpecialMode = std::make_shared<GUI::Label>("Special Mode");
    textSpecialMode->setCharacterSize(30);
    textSpecialMode->setColor(sf::Color::Yellow);
    textSpecialMode->setPosition((1400 - textSpecialMode->getWidth()) / 2, 300 * bonusY);    



    nGUIContainer.pack(slotButton1);
    nGUIContainer.pack(slotButton2);
    nGUIContainer.pack(slotButton3);
    nGUIContainer.pack(textNormalMode);
    nGUIContainer.pack(textSpecialMode);
    nGUIContainer.pack(creativeButton);
}

void ChooseModeState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(nBackgroundSprite);
    window.draw(nGUIContainer);
    window.draw(backButton);
}

bool ChooseModeState::update(sf::Time dt)
{
    nGUIContainer.update(dt);
    return true;
}

bool ChooseModeState::handleEvent(const sf::Event& event)
{
    nGUIContainer.handleEvent(event);
    backButton.handleEvent(event);
    return false;
}
