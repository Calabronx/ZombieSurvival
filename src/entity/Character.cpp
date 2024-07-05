#include "Character.h"
#include "../util/FileSystem.h"
#include "../util/Category.h"

Textures::ID toTextureId(Character::Type type)
{
	switch (type)
	{
			case Character::Survivor:
				return Textures::Survivor;

			case Character::Zombie:
				return Textures::Zombie;
	}
	return Textures::Survivor;
}

Character::Character(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(toTextureId(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Character::getCategory() const
{
    switch (mType)
    {
        case Survivor:
            return Category::PlayerSurvivor;

        default:
            return Category::Zombie;
    }
}