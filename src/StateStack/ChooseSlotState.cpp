#include "ChooseSlotState.hpp"
#include "../UI/Button.hpp"
#include "../UI/Label.hpp"
#include <fstream>


ChooseSlotState::ChooseSlotState(StateStack& stack, Context context)
: State(stack, context)
, nBackgroundSprite()
, nGUIContainerSlot()
, nGUIContainerConfirm()
, nSelectedSlot(-1)
, isConfirm(false)
, nameBox(context)
, backButton(context, Textures::ID::BackButtonNormal, Textures::ID::BackButtonSelected, Textures::ID::BackButtonPressed)
, settingButton(context, Textures::ID::SettingButtonNormal, Textures::ID::SettingButtonSelected, Textures::ID::SettingButtonPressed)
, nameSlot1(std::make_shared<GUI::Label>("NEW !"))
, nameSlot2(std::make_shared<GUI::Label>("NEW !"))
, nameSlot3(std::make_shared<GUI::Label>("NEW !"))
{
    nBackgroundSprite.setTexture(TextureHolder::getInstance().get(Textures::ChooseModeScreen));


    std::string filename = *context.saveFile;

    auto slotButton1 = std::make_shared<GUI::Button>(context, Textures::ID::SlotButtonNormal, Textures::ID::SlotButtonSelected, Textures::ID::SlotButtonPressed);
    slotButton1->setPosition(373, 400.5);
    nameSlot1->setPosition(373, 400.5);
    nameSlot1->setCharacterSize(22);
    adjust(nameSlot1, filename + "Save1/");
    slotButton1->setSizeText(22);
    slotButton1->setCallback([this] ()
    {
        nSelectedSlot = 1;
    });



    auto slotButton2 = std::make_shared<GUI::Button>(context, Textures::ID::SlotButtonNormal, Textures::ID::SlotButtonSelected, Textures::ID::SlotButtonPressed);
    slotButton2->setPosition(632, 400.5);
    nameSlot2->setPosition(632, 400.5);
    nameSlot2->setCharacterSize(22);
    adjust(nameSlot2, filename + "Save2/");
    slotButton2->setSizeText(22);
    slotButton2->setCallback([this] ()
    {
        nSelectedSlot = 2;
    });

    auto slotButton3 = std::make_shared<GUI::Button>(context, Textures::ID::SlotButtonNormal, Textures::ID::SlotButtonSelected, Textures::ID::SlotButtonPressed);
    slotButton3->setPosition(891, 400.5);
    nameSlot3->setPosition(891, 400.5);
    nameSlot3->setCharacterSize(22);
    adjust(nameSlot3, filename + "Save3/");
    slotButton3->setSizeText(22);
    slotButton3->setCallback([this] ()
    {
        nSelectedSlot = 3;
    });

    backButton.setPosition({75, 705});
    backButton.setIsSelected(false);
    backButton.setCallback([this] ()
    {
        requestStackPop();
        requestStackPush(States::Title);
        requestStackPush(States::ID::Transition);

    });

    settingButton.setPosition({75, 92});
    settingButton.setIsSelected(false);
    settingButton.setCallback([this] ()
    {
        requestStackPush(States::Instruction);
        requestStackPush(States::ID::Transition);
    });

    auto startButton = std::make_shared<GUI::Button>(context, Textures::ID::StartButtonNormal, Textures::ID::StartButtonSelected, Textures::ID::StartButtonPressed);    
    startButton->setPosition(373, 615);
    startButton->setCallback([this] ()
    {
        State::Context context = getContext();
        std::string filename = *context.saveFile + "Save" + std::to_string(nSelectedSlot) + "/" + "state.bin";
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            isConfirm = true;
            nameBox.setChoose(true);            
        }
        else {
            int idState;
            file.read(reinterpret_cast<char*>(&idState), sizeof(idState));
            file.close();
            requestStackPop();
            requestStackPush(States::ID(idState));
            requestStackPush(States::ID::Transition);
            *context.saveFile = *context.saveFile + "Save" + std::to_string(nSelectedSlot) + "/";
        }
        // requestStackPop();
        // requestStackPush(States::ID::ChoosePlayer);
        // requestStackPush(States::Transition);
    });

    auto deleteButton = std::make_shared<GUI::Button>(context, Textures::ID::DeleteButtonNormal, Textures::ID::DeleteButtonSelected, Textures::ID::DeleteButtonPressed);
    deleteButton->setPosition({891, 615});
    deleteButton->setCallback([this] ()
    {
        // delete file
        State::Context context = getContext();
        clearFolder(*context.saveFile + "Save" + std::to_string(nSelectedSlot));
    });

    auto resetButton = std::make_shared<GUI::Button>(context, Textures::ID::ResetButtonNormal, Textures::ID::ResetButtonSelected, Textures::ID::ResetButtonPressed);
    resetButton->setPosition({632, 615});
    resetButton->setCallback([this] ()
    {
        nSelectedSlot = -1;
        nGUIContainerConfirm.selectPrevious();
    });
    // pass
    nameBox.setPosition({536, 366});
    nameBox.setTitle("Enter your username");

    nGUIContainerSlot.pack(slotButton1);
    nGUIContainerSlot.pack(slotButton2);
    nGUIContainerSlot.pack(slotButton3);

    nGUIContainerConfirm.pack(resetButton);
    nGUIContainerConfirm.pack(deleteButton);
    nGUIContainerConfirm.pack(startButton);
    nGUIContainerConfirm.selectNext();
    nGUIContainerConfirm.selectNext();
}

ChooseSlotState::~ChooseSlotState() {
    
}

void ChooseSlotState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(nBackgroundSprite);
    window.draw(nGUIContainerSlot);
    window.draw(backButton);
    window.draw(settingButton);
    window.draw(*nameSlot1);
    window.draw(*nameSlot2);
    window.draw(*nameSlot3);
    if (nSelectedSlot != -1)
        window.draw(nGUIContainerConfirm);
    if (isConfirm) {
        sf::RectangleShape rect(window.getView().getSize());
        rect.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(rect);
        window.draw(nameBox);
    }
}

bool ChooseSlotState::update(sf::Time dt)
{
    sf::RenderWindow& window = *getContext().window;
    adjust(nameSlot1, *getContext().saveFile + "Save1/");
    adjust(nameSlot2, *getContext().saveFile + "Save2/");
    adjust(nameSlot3, *getContext().saveFile + "Save3/");
    
    if (isConfirm == false) {
        if (backButton.isMouseOver(window))
            backButton.setSelectedSprite();
        else backButton.setNormalSprite();
        if (settingButton.isMouseOver(window))
            settingButton.setSelectedSprite();
        else settingButton.setNormalSprite();
    }
    else {
        nameBox.update(dt);
    }
    return true;
}

bool ChooseSlotState::handleEvent(const sf::Event& event)
{   
    if (isConfirm == false) {
        if (nSelectedSlot != -1) {
            nGUIContainerConfirm.handleEvent(event);
        }
        else {
            nGUIContainerSlot.handleEvent(event);
        }
        backButton.handleEvent(event);
        settingButton.handleEvent(event);
    }
    else {
        nameBox.handleEvent(event);
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                int length = nameBox.getInput().size();
                if (length > 0) {
                    State::Context context = getContext();
                    std::string filename = *context.saveFile + "Save" + std::to_string(nSelectedSlot) + "/" + "name.bin";
                    std::ofstream file(filename, std::ios::binary);
                    file.write((char*)&length, sizeof(int));
                    file.write(nameBox.getInput().c_str(), length);
                    file.close();

                    filename = *context.saveFile + "Save" + std::to_string(nSelectedSlot) + "/" + "map.bin";
                    std::ofstream map(filename, std::ios::binary);
                    for (int i = 0; i < 6; i++) {
                        bool ok = false;
                        map.write((char*)&ok, sizeof(bool));
                    }

                    map.close();

                    requestStackPop();
                    requestStackPush(States::ChooseMode);
                    requestStackPush(States::ID::Transition);
                    *context.saveFile = *context.saveFile + "Save" + std::to_string(nSelectedSlot) + "/";
                }
            }
            if (event.key.code == sf::Keyboard::Escape) {
                isConfirm = false;
            }
        }
    }
    return false;
}

void ChooseSlotState::adjust(GUI::Label::Ptr label, std::string filename)
{
    std::ifstream file(filename + "name.bin", std::ios::binary);
    if (!file) {
        label->setText("NEW !");
    }
    else {
        int length;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        std::string name;
        name.resize(length);
        file.read(&name[0], length);
        label->setText(name);
        file.close();
    }
}

void ChooseSlotState::saveCurrentState() {
    Context context = getContext();
    std::ofstream savefile(*context.saveFile + "state.bin", std::ios::binary);
    assert(savefile.is_open());
    int state = States::ID::ChooseSlot;
    savefile.write((char*)&state, sizeof(int));
    savefile.close();
}
