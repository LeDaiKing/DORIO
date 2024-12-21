#include "CheckPoint.hpp"
#include "../Holder/ResourceIdentifiers.hpp"
#include "../Holder/ResourceHolder.hpp"
#include "Dough.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
// #include "../Holder/TextureHolder.hpp"

Textures::ID toTextureID(CheckPoint::Type type)
{
    switch (type)
    {
    case CheckPoint::Start:
        return Textures::Start;
    case CheckPoint::Checkpoint:
        return Textures::Checkpoint;
    case CheckPoint::End:
        return Textures::End;
    default:
        return Textures::Start;
    }
}

CheckPoint::CheckPoint(Type type, sf::Vector2f position)
: nSprite(TextureHolder::getInstance().get(toTextureID(type)))
, nType(type)
{
    nSprite.setPosition(position);
    centerOrigin(nSprite);
    switch (type)
    {
    case Start:
        nSprite.addAnimationState(0, 0, 17, sf::seconds(1.f), sf::Vector2i(64, 64), true);
        break;
    case End:
        nSprite.addAnimationState(0, 0, 8, sf::seconds(0.7), sf::Vector2i(64, 64), true);
        break;
    
    case Checkpoint:
        nSprite.addAnimationState(0, 128, 1, sf::seconds(0.7), sf::Vector2i(64, 64), true);
        nSprite.addAnimationState(1, 64, 26, sf::seconds(0.6), sf::Vector2i(64, 64), false);
        nSprite.addAnimationState(2, 0, 10, sf::seconds(0.7), sf::Vector2i(64, 64), true);
        break;
    default:
        break;
    }
    nSprite.setAnimationState(0);
}

// unsigned int CheckPoint::getCategory() const
// {
//     return Category::
// }

sf::FloatRect CheckPoint::getBoundingRect() const
{
    return getWorldTransform().transformRect(nSprite.getGlobalBounds());
}

void CheckPoint::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (nType == Checkpoint || nType == End)
    {
        Command checkPoint;
        checkPoint.category = Category::PlayerDough;
        checkPoint.action = derivedAction<Dough>([this](Dough& player, sf::Time)
        {
            if (player.getBoundingRect().intersects(getBoundingRect()))
            {
                if (nType == End)
                {
                    //Winning
                }
                else if (nSprite.getCurrentAnimationID() == 0)
                {
                    nSprite.setAnimationState(1);
                    player.setCheckPoint(nSprite.getPosition());
                }
            }
        });
        commands.push(checkPoint);
    }

    if (nType == Checkpoint && nSprite.isFinished())
    {
        nSprite.setAnimationState(2);
    }

    nSprite.update(dt);
}

void CheckPoint::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(nSprite, states);
}

void CheckPoint::save(std::ofstream& file)
{
    file.write(reinterpret_cast<const char*>(&nType), sizeof(nType));
    sf::Vector2f pos = nSprite.getPosition();
    file.write(reinterpret_cast<char*>(&pos), sizeof(pos));
    int currentAnimation = nSprite.getCurrentAnimationID();
    file.write(reinterpret_cast<char*>(&currentAnimation), sizeof(currentAnimation));
}

void CheckPoint::load(std::ifstream& file)
{
    int currentAnimation;
    file.read(reinterpret_cast<char*>(&currentAnimation), sizeof(currentAnimation));
    nSprite.setAnimationState(currentAnimation);
}