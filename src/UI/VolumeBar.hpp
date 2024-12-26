#pragma once
#include "Component.hpp"
#include "Button.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <vector>
#include <memory>


namespace GUI{
    class VolumeBar : public Component {
        public:
        typedef std::shared_ptr<VolumeBar> Ptr;

        public: 
        VolumeBar(State::Context context);
        virtual void update(sf::Time dt);
        virtual void handleEvent(const sf::Event& event);
        virtual void increaseVolume();
        virtual void decreaseVolume();
        float getVolume() const;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool isSelectable() const;
        virtual void setVolume(float volume);

        private:
        const int MAX_BLOCK = 3;
        const float START_X = 105;
        const float START_Y = 2;
        const float BLOCK_SIZE = 80;
        int currentBlock;
        sf::Sprite normalBlock[3];
        GUI::Button increaseButton;
        GUI::Button decreaseButton;

    };
}