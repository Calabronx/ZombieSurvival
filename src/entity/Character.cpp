#include "Character.h"
#include "data/DataTables.h"
#include "../util/FileSystem.h"
#include "../util/Category.h"
#include "../util/Utility.h"
#include "Pickup.h"
#include <iostream>

namespace {
	const std::vector<CharacterData> Table = initializeCharacterData();
}

//Textures::ID toTextureId(Character::Type type)
//{
//	switch (type)
//	{
//	case Character::Survivor:
//		return Textures::Survivor;
//
//	case Character::Zombie:
//		return Textures::Zombie;
//	}
//	return Textures::Survivor;
//}

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mBlood(textures.get(Textures::Blood))
	//, mTextureFrames(textures)
	//, mZombieAnim(textures.get(Textures::ZombieWalk))
	, mTextureAnimations()
	, mDirectionAngle(0.0f)
	, mHealthDisplay(nullptr)
	, mZombieTargetDirection()
	, mIsFiring(false)
	, mIsReloading(false)
	, mSpawnedPickup(false)
	, mShowBlood(true)
	, mIsMarkedForRemoval(false)
	, mFireCountdown(sf::Time::Zero)
	, mFireRateLevel(1)
	, mSpreadLevel(1)
	, mFireCommand()
	, mDropPickupCommand()
	, mReloadCommand()
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
	, mGunPosition()
	, mCurrentFrame(0)
	, mElapsedFrameTime(sf::Time::Zero)
	, mLeftTexture(0)
	, mWidthTexture(0)
	, mHeightTexture(0)
	, mAmmo(60)
	, mPlayerHealth()
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	//sf::FloatRect gun = bounds.top + bounds.left;
	sf::Vector2f center(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	mCenter = center;

	centerOrigin(mSprite);
	centerOrigin(mBlood);
	mBlood.setFrameSize(sf::Vector2i(156, 156));
	mBlood.setNumFrames(4);
	mBlood.setDuration(sf::seconds(1));
	mBlood.setScale(sf::Vector2f(4.f, 4.f));
	//mBlood.setPosition(mSprite.getPosition());

	//mTextures = textures;
	//mTextureAnimations.push_back(textures.get(Textures::Reload));



	//mTextureFrames = textures;
	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();

	if (getCategory() == Category::PlayerSurvivor) {
		mTextureAnimations.push_back(textures.get(Textures::Survivor));
		mTextureAnimations.push_back(textures.get(Textures::Shoot));
		mTextureAnimations.push_back(textures.get(Textures::Reload));

		setRotation(mDirectionAngle);

		mFireCommand.category = Category::SceneLandLayer;
		mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
		{
			createBullets(node, textures);
		};

	}

	if (getCategory() == Category::Zombie) {
		attachChild(std::move(healthDisplay));
	}

	mDropPickupCommand.category = Category::SceneLandLayer;
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};


	updateTexts();

}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowBlood)
		target.draw(mBlood, states);
	
	else
		target.draw(mSprite, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// update texts
	updateTexts();

	if (isDestroyed())
	{
		checkPickupDrop(commands);
		mBlood.update(dt);
		return;
	}

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

			int numFrames = 18;
			bool repeat = true;
			sf::Vector2i frameSize(220, 220);
			sf::Time duration = sf::seconds(2);
			sf::Time timePerFrame = duration / static_cast<float>(numFrames);
			mElapsedFrameTime += dt;
			sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
			sf::IntRect textureRect = Table[mType].textureRect;
			int left = 0;

			if (mCurrentFrame == 0)
				textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);

			while (mElapsedFrameTime >= timePerFrame && (mCurrentFrame <= numFrames || repeat))
			{
				textureRect.left += textureRect.width;

				if (textureRect.left + textureRect.width > textureBounds.x)
				{
					textureRect.left = 0;
					textureRect.top += textureRect.height;
				}

				mElapsedFrameTime -= timePerFrame;
				if (repeat)
				{
					mCurrentFrame = (mCurrentFrame + 1) % numFrames;

					if (mCurrentFrame == 0)
						textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
				}
				else
				{
					mCurrentFrame++;
				}
				/*	std::cout << "FRAME: " << mCurrentFrame << std::endl;
					std::cout << "w: " << textureRect.getPosition().x << std::endl;
					std::cout << "y: " << textureRect.getPosition().y << std::endl;*/

			}

			mSprite.setTextureRect(textureRect);

		}
	}
	else if (getCategory() == Category::PlayerSurvivor)
	{
		if (mIsReloading) {
			//std::cout << "RELOAD! : ammo : " << mAmmoCounter << std::endl;
			commands.push(mReloadCommand);
			mIsReloading = false;

			//if (mAction != RELOAD)
			mSprite.setTexture(mTextureAnimations[2]);
			mAction = RELOAD;

			updatePlayerAnimation(dt);
			return;

		}
	}

	// check if bullets are fired
	checkProjectileLaunch(dt, commands);

	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

}


bool Character::isMarkedForRemoval() const
{
	return isDestroyed() && (mBlood.isFinished() || !mShowBlood);
}

void Character::remove()
{
	Entity::remove();
	mShowBlood = false;
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

void Character::updatePlayerAnimation(sf::Time dt)
{
	int numFrames = 18;
	bool repeat = false;
	bool reload = false;

	sf::Time duration;
	sf::Vector2i frameSize;
	sf::IntRect textureRect;
	//mElapsedFrameTime = sf::Time::Zero;
	switch (mAction) {
	case SHOOT:
		textureRect = sf::IntRect(0, 0, 300, 200);
		frameSize = sf::Vector2i(300, 300);
		duration = sf::seconds(0.200);
		repeat = true;
		break;
	case RELOAD:
		textureRect = sf::IntRect(0, 0, 310, 250);
		frameSize = sf::Vector2i(310, 250);
		duration = sf::seconds(0.800);
		numFrames = 20;
		mCurrentFrame = 0;
		mIsReloading = false;
		//mSprite.setTexture(mTextureAnimations[2]);
		//mElapsedFrameTime = sf::Time::Zero;
		std::cout << "reloading" << std::endl;
		break;
	case MOVE:
		break;
	default:
		break;
	}

	sf::Time timePerFrame = duration / static_cast<float>(numFrames); // aca esta el bug
	mElapsedFrameTime += dt;
	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());

	if (mCurrentFrame == 0)
		textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);

	// While we have a frame to process
	while (mElapsedFrameTime >= timePerFrame && (mCurrentFrame <= numFrames || repeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		mElapsedFrameTime -= timePerFrame;
		if (repeat)
		{
			mCurrentFrame = (mCurrentFrame + 1) % numFrames;

			if (mCurrentFrame == 0)
				textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
		}
		else
		{
			mCurrentFrame++;
		}
		/*	std::cout << "FRAME: " << mCurrentFrame << std::endl;
			std::cout << "w: " << textureRect.getPosition().x << std::endl;
			std::cout << "y: " << textureRect.getPosition().y << std::endl;*/

	}

	mSprite.setTextureRect(textureRect);
}

void Character::updateTexts()
{
	if (getCategory() == Category::PlayerSurvivor) {
		//mPlayerHealth.setString(toString(getHitpoints()) + " HP");
		//mPlayerHealth.setPosition(360.f, 4760.f);
		//mPlayerHealth.setCharacterSize(10u);
	}
	else
	{
		mHealthDisplay->setString(toString(getHitpoints()) + " HP");
		mHealthDisplay->setPosition(0.f, 95.f);
		mHealthDisplay->setRotation(-getRotation());
	}

}

void Character::checkPickupDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(3) == 0 && !mSpawnedPickup)
		commands.push(mDropPickupCommand);

	mSpawnedPickup = true;
}

void Character::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	//if (mIsReloading) {
	//	//std::cout << "RELOAD! : ammo : " << mAmmoCounter << std::endl;
	//	commands.push(mReloadCommand);
	//	mIsReloading = false;

	//	if (mAction != RELOAD)
	//		mSprite.setTexture(mTextureAnimations[2]);
	//		mAction = RELOAD;

	//	updatePlayerAnimation(dt);
	//	return;

	//}

	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;

		if (mAction != SHOOT)
			mSprite.setTexture(mTextureAnimations[1]);
		mAction = SHOOT;
		mAmmo--;
		/*mAmmoFired++;*/

		updatePlayerAnimation(dt);
		//std::cout << "FIRE! : ammo shoot: " << mAmmoFired << std::endl;

	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
		mAction = MOVE;

		if (mAction != MOVE)
			//mSprite.setTexture(mTextureAnimations[0]);
			mAction = MOVE;
	}
}

void Character::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = Projectile::HandgunBullet;
	float rotation = getRotation();
	//std::cout << rotation << std::endl;

	float xOffset = rotation >= 0.f && rotation >= 230 ? -0.2f : rotation < 230 && rotation >= 199 ? -0.4f
		: rotation < 199 && rotation >= 130 ? -0.2f : rotation < 170 && rotation >= 110 ? 0.0f : 0.0;

	float yOffset = rotation >= 0.f && rotation >= 230 ? 0.0f : rotation < 199 && rotation >= 170 ? 0.1f : rotation < 170 && rotation >= 110 ? 0.4f : 0.0;

	//createProjectile(node, type, -0.2f, 0.0f, textures); // 316 - 253
	//createProjectile(node, type, 0.2f, 0.0f, textures); // 253 - 230
	//createProjectile(node, type, -0.4f, 0.0f, textures); // 214 - 199

	createProjectile(node, type, xOffset, yOffset, textures);
}

void Character::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);

	sf::Vector2f center(mSprite.getGlobalBounds().width / 2.f, mSprite.getGlobalBounds().height / 2.f);

	sf::Vector2f mouseWorldPosition = mMousePosition;
	float sign = -0.30f;

	projectile->setPosition(getWorldPosition() + offset * sign);
	float mouseAngle = std::atan2(mMousePosition.y - projectile->getPosition().y, mMousePosition.x - projectile->getPosition().x);
	float bulletRotation = std::atan2(mMousePosition.y, mMousePosition.x);
	float mouse = std::atan2(mouseWorldPosition.y, mouseWorldPosition.x);
	float degrees = toDegree(mouseAngle) + 90;

	projectile->setVelocity(sf::Vector2f(projectile->getMaxSpeed() * std::cos(mouseAngle), projectile->getMaxSpeed() * std::sin(mouseAngle)));
	projectile->setRotation(degrees); // falta ajustar el angulo perfecto acorde al cursor, solo en y+/y- la bala va derecha segun el angulo
	projectile->setScale(0.019999f, 0.2f);
	node.attachChild(std::move(projectile));
	//std::cout << "BULLET VEC SHOOT POS (X: " << projectile->getPosition().x << ",Y: " << projectile->getPosition().y << ")" << std::endl;
}

void Character::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 0.f);
	node.attachChild(std::move(pickup));
}

unsigned int Character::getCategory() const
{
	if (isAllied())
		return Category::PlayerSurvivor;

	else
		return Category::Zombie;

}

bool Character::isAllied() const
{
	return mType == Survivor;
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

sf::Vector2f Character::getGunPosition() const
{
	float angleRad = toRadian(getRotation());
	sf::Vector2f offset(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height);

	sf::Vector2f gunOffset(mSprite.getPosition().x * cos(angleRad) - mSprite.getPosition().y * sin(angleRad),
		mGunPosition.x * sin(angleRad) * mGunPosition.y * cos(angleRad));

	return getPosition() + gunOffset;
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

void Character::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}

void Character::increaseSpread()
{
	if (mSpreadLevel < 3)
		++mSpreadLevel;
}


void Character::fire()
{
	if (mAmmo <= 0)
		return;


	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;

	//std::cout << "fire" << std::endl;
}

void Character::reload()
{
	if (mAmmo >= 60)
		return;

	mIsReloading = true;
	mAmmo = 60;
}


