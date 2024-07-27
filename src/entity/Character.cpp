#include "Character.h"
#include "../util/FileSystem.h"
#include "../util/Category.h"
#include <iostream>

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
	, mDirectionAngle(0.0f)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	sf::Vector2f center(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	mCenter = center;

	if (type == Survivor)
		setRotation(mDirectionAngle);

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

void Character::setDirectionAngle(float angle)
{
	mDirectionAngle = angle;
}

void Character::moveAim()
{
	setRotation(mDirectionAngle);
}

sf::FloatRect Character::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}
