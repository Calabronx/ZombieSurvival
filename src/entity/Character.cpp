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
	, mZombieTargetDirection()
	, mIsFiring(false)
	, mFireCommand()
	, mFireCountdown(sf::Time::Zero)
	, mFireRateLevel(1)
	, mSpreadLevel(1)
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	sf::Vector2f center(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	mCenter = center;

	if (getCategory() == Category::PlayerSurvivor) {
		setRotation(mDirectionAngle);

		mFireCommand.category = Category::SceneLandLayer;
		mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
		{
			createBullets(node, textures);
		};
	}
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

	if (getCategory() == Category::Zombie)
	{
		if (isChasing())
		{
			const float approachRate = 200.f;

			sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mZombieTargetDirection + getVelocity());
			newVelocity *= getMaxSpeed();
			float angle = std::atan2(newVelocity.y, newVelocity.x);

			setRotation(toDegree(angle) + 90.f);
			setVelocity(newVelocity);
		}
	}

	// check if bullets are fired
	checkProjectileLaunch(dt, commands);

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

void Character::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}
}

void Character::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::HandgunBullet;

	switch (mSpreadLevel)
	{
			case 1:
				createProjectile(node, type, 0.0f, 0.5f, textures);
				break;

			case 2:
				createProjectile(node, type, -0.33f, 0.33f, textures);
				createProjectile(node, type, +0.33f, 0.33f, textures);
				break;

			case 3:
				createProjectile(node, type, -0.5f, 0.33f, textures);
				createProjectile(node, type,  0.0f, 0.5f,  textures);
				createProjectile(node, type, +0.5f, 0.33f, textures);
				break;
	}
}

void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f mouseWorldPosition = mMousePosition;
	float sign = -0.30f;

	projectile->setPosition(getWorldPosition() + offset * sign);
	float mouseAngle = std::atan2(mMousePosition.y - projectile->getPosition().y, mMousePosition.x - projectile->getPosition().x);
	float bulletRotation = std::atan2(mMousePosition.y, mMousePosition.x);

	projectile->setVelocity(sf::Vector2f(projectile->getMaxSpeed() * std::cos(mouseAngle), projectile->getMaxSpeed() * std::sin(mouseAngle)));
	projectile->setRotation(toRadian(bulletRotation)); // falta ajustar el angulo perfecto acorde al cursor, solo en y+/y- la bala va derecha segun el angulo
	//std::cout << "MOUSE VEC (X: " << mouseWorldPosition.x << ",Y: " << mouseWorldPosition.y << ")" << std::endl;
	//std::cout << "BULLET VEC TRAYECTORY (X: " << projectile->getVelocity().x << ",Y: " << projectile->getVelocity().y << ")" << std::endl;
	node.attachChild(std::move(projectile));
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

void Character::setMousePosition(sf::Vector2f mousePosition)
{
	mMousePosition = mousePosition;
}

float Character::getDirectionAngle() const
{
	return getRotation();
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

void Character::guideTowardsPlayer(sf::Vector2f position)
{
	if (mType != Type::Zombie)
	{
		return;
	}

	mZombieTargetDirection = unitVector(position - getWorldPosition());
}

bool Character::isChasing() const
{
	return mType == Zombie;
}

void Character::fire()
{
	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;

	//std::cout << "fire" << std::endl;
}
