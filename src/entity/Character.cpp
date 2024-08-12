#include "Character.h"
#include "data/DataTables.h"
#include "../util/FileSystem.h"
#include "../util/Category.h"
#include "../util/Utility.h"
#include <iostream>

namespace {
	const std::vector<CharacterData> Table = initializeCharacterData();
}

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

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(toTextureId(type)))
	, mDirectionAngle(0.0f)
	, mHealthDisplay(nullptr)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	sf::Vector2f center(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	mCenter = center;

	if (type == Survivor)
		setRotation(mDirectionAngle);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	updateTexts();

}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

	// update texts
	updateTexts();
}

void Character::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		float distanceToTravel = directions[mDirectionIndex].distance;
		if (mTravelledDistance > distanceToTravel)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Character::updateTexts()
{
	mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 95.f);
	mHealthDisplay->setRotation(-getRotation());
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

float Character::getMaxSpeed() const
{
	return Table[mType].speed;
}

sf::FloatRect Character::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}
