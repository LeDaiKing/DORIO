#pragma once
#include "Component.hpp"
#include <vector>
#include <memory>


namespace GUI{
    class Container : public Component {
        public:
        typedef std::shared_ptr<Container> Ptr;

        public: 
        Container();
        void pack(Component::Ptr component, bool isAuto = false);
        
        virtual void update(sf::Time dt);
        virtual bool isSelectable() const;
        virtual bool isPressasble() const;
        virtual void handleEvent(const sf::Event& event);
        void selectNext();
        void selectPrevious();
        Component::Ptr getSelectedChild() const;

        private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        bool hasSelection() const;
        void select(std::size_t index);

        private:
        std::vector<Component::Ptr> nChildren;
        std::vector<bool> nIsAuto;
        int nSelectedChild;
    };
}