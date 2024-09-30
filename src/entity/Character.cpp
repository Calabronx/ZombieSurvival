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

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mBloodAnim(textures.get(Textures::Blood))
	, mHealthDisplay(nullptr)
	, mSpawnedPickup(false)
	, mShowBlood(true)
	, mIsMarkedForRemoval(false)
	, mFireCommand()
	, mDropPickupCommand()
	, mReloadCommand()
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
	, mGunPosition()
	, mCurrentFrame(0)
	, mElapsedFrameTime(sf::Time::Zero)
	, mAmmo(16)
	, mPlayerHealth()
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	sf::Vector2f center(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	mCenter = center;

	centerOrigin(mSprite);
	mBloodAnim.setFrameSize(sf::Vector2i(156, 156));
	mBloodAnim.setNumFrames(4);
	mBloodAnim.setDuration(sf::seconds(1));
	mBloodAnim.setScale(sf::Vector2f(4.f, 4.f));
	centerOrigin(mBloodAnim);
	mBloodAnim.setPosition(mBloodAnim.getPosition() + sf::Vector2f(+150.f, +300.0f));

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();

	if (getCategory() == Category::PlayerSurvivor) {

		setRotation(mDirectionAngle);

		mFireCommand.category = Category::SceneLandLayer;
		mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
		{
			createBullets(node, textures);
		};
		// Rifle animations
		mRifleShootAnim.setFrameSize(sf::Vector2i(300, 300));
		mRifleShootAnim.setNumFrames(3);
		mRifleShootAnim.setDuration(sf::seconds(0.0005));
		mRifleShootAnim.setTexture(textures.get(Textures::RifleShoot));
		mRifleShootAnim.setRepeating(true);
		mRifleShootAnim.setPosition(mRifleShootAnim.getPosition() - sf::Vector2f(0.f, -50.f));
		centerOrigin(mRifleShootAnim);
		mRifleReloadAnim.setFrameSize(sf::Vector2i(330, 220));
		mRifleReloadAnim.setNumFrames(14);
		mRifleReloadAnim.setDuration(sf::seconds(2));
		mRifleReloadAnim.setTexture(textures.get(Textures::RifleReload));
		mRifleReloadAnim.setRepeating(false);
		centerOrigin(mRifleReloadAnim);
		mRifleIdleAnim.setFrameSize(sf::Vector2i(311, 210));
		mRifleIdleAnim.setNumFrames(18);
		mRifleIdleAnim.setDuration(sf::seconds(3));
		mRifleIdleAnim.setTexture(textures.get(Textures::RifleIdle));
		mRifleIdleAnim.setRepeating(true);
		centerOrigin(mRifleIdleAnim);
		mRifleMoveAnim.setFrameSize(sf::Vector2i(311, 210));
		mRifleMoveAnim.setNumFrames(20);
		mRifleMoveAnim.setDuration(sf::seconds(2));
		mRifleMoveAnim.setTexture(textures.get(Textures::RifleMove));
		mRifleMoveAnim.setRepeating(true);
		centerOrigin(mRifleMoveAnim);
		// Handgun animations
		mHandgunIdleAnim.setFrameSize(sf::Vector2i(250, 210));
		mHandgunIdleAnim.setNumFrames(18);
		mHandgunIdleAnim.setDuration(sf::seconds(3));
		mHandgunIdleAnim.setTexture(textures.get(Textures::HandgunIdle));
		mHandgunIdleAnim.setRepeating(true);
		centerOrigin(mHandgunIdleAnim);
		mHandgunShootAnim.setFrameSize(sf::Vector2i(300, 300));
		mHandgunShootAnim.setNumFrames(3);
		mHandgunShootAnim.setDuration(sf::seconds(0.0005));
		mHandgunShootAnim.setTexture(textures.get(Textures::HandgunShoot));
		mHandgunShootAnim.setRepeating(true);
		mHandgunShootAnim.setPosition(mHandgunShootAnim.getPosition() - sf::Vector2f(0.f, -50.f));
		centerOrigin(mHandgunShootAnim);
		/*	mHandgunReloadAnim.setFrameSize(sf::Vector2i(330, 220));
			mHandgunReloadAnim.setNumFrames(14);
			mHandgunReloadAnim.setDuration(sf::seconds(2));
			mHandgunReloadAnim.setTexture(textures.get(Textures::HandgunReload));
			mHandgunReloadAnim.setRepeating(false);
			centerOrigin(mHandgunReloadAnim);*/
		mHandgunMoveAnim.setFrameSize(sf::Vector2i(311, 210));
		mHandgunMoveAnim.setNumFrames(20);
		mHandgunMoveAnim.setDuration(sf::seconds(2));
		mHandgunMoveAnim.setTexture(textures.get(Textures::HandgunMove));
		mHandgunMoveAnim.setRepeating(true);
		// Shotgun animations
		mShotgunShootAnim.setFrameSize(sf::Vector2i(300, 300));
		mShotgunShootAnim.setNumFrames(3);
		mShotgunShootAnim.setDuration(sf::seconds(0.0005));
		mShotgunShootAnim.setTexture(textures.get(Textures::ShotgunShoot));
		mShotgunShootAnim.setRepeating(true);
		centerOrigin(mShotgunShootAnim);
		mShotgunShootAnim.setPosition(mShotgunShootAnim.getPosition() - sf::Vector2f(0.f, -50.f));
		/*	mShotgunReloadAnim.setFrameSize(sf::Vector2i(330, 220));
			mShotgunReloadAnim.setNumFrames(14);
			mShotgunReloadAnim.setDuration(sf::seconds(2));
			mShotgunReloadAnim.setTexture(textures.get(Textures::ShotgunReload));
			mShotgunReloadAnim.setRepeating(false);
			centerOrigin(mShotgunReloadAnim);*/
		mShotgunIdleAnim.setFrameSize(sf::Vector2i(311, 210));
		mShotgunIdleAnim.setNumFrames(18);
		mShotgunIdleAnim.setDuration(sf::seconds(3));
		mShotgunIdleAnim.setTexture(textures.get(Textures::ShotgunIdle));
		mShotgunIdleAnim.setRepeating(true);
		centerOrigin(mShotgunIdleAnim);
		mShotgunMoveAnim.setFrameSize(sf::Vector2i(311, 210));
		mShotgunMoveAnim.setNumFrames(20);
		mShotgunMoveAnim.setDuration(sf::seconds(2));
		mShotgunMoveAnim.setTexture(textures.get(Textures::ShotgunMove));
		mShotgunMoveAnim.setRepeating(true);

		mIsFiring = false;
		mIsReloading = false;
		mIsMoving = false;
		mFireCountdown = sf::Time::Zero;
		mFireRateLevel = 1;
		mDirectionAngle = 0.0f;
		mAction = IDLE;
		mProjectileType = Projectile::Type::HandgunBullet;
		mGunEquipped = 1;
	}

	if (getCategory() == Category::Zombie) {
		mZombieMoveAnim.setFrameSize(sf::Vector2i(278, 320));
		mZombieMoveAnim.setNumFrames(18);
		mZombieMoveAnim.setDuration(sf::seconds(2));
		mZombieMoveAnim.setTexture(textures.get(Textures::ZombieWalk));
		mZombieMoveAnim.setRepeating(true);
		centerOrigin(mZombieMoveAnim);

		mZombieAttackAnim.setFrameSize(sf::Vector2i(300, 290));
		mZombieAttackAnim.setNumFrames(9);
		mZombieAttackAnim.setDuration(sf::seconds(1));
		mZombieAttackAnim.setTexture(textures.get(Textures::ZombieAttack));
		mZombieAttackAnim.setRepeating(true);
		centerOrigin(mZombieAttackAnim);

		mIsZombieAttacking = false;
		mIsZombieChasing = false;

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
		target.draw(mBloodAnim, states);
	else if (getCategory() == Category::PlayerSurvivor)
		target.draw(getGunAnimationObj(mGunEquipped, mAction), states);
	else if (getCategory() == Category::Zombie)
		if (mAction == MOVE)
			target.draw(mZombieMoveAnim, states);
		else if (mAction == ATTACK)
			target.draw(mZombieAttackAnim, states);
}

void Character::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// update texts
	updateTexts();

	if (isDestroyed())
	{
		checkPickupDrop(commands);
		mBloodAnim.update(dt);
		return;
	}

	if (getCategory() == Category::Zombie)
	{
		if (isChasing())
		{
			if (mIsZombieAttacking) {
				mZombieAttackAnim.update(dt);
				mAction = ATTACK;
			}
			else {
				mZombieMoveAnim.update(dt);
				mAction = MOVE;
			}

			const float approachRate = 200.f;
			sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mZombieTargetDirection + getVelocity());
			newVelocity *= getMaxSpeed();
			float angle = std::atan2(newVelocity.y, newVelocity.x);

			setRotation(toDegree(angle));
			setVelocity(newVelocity);


		}
	}
	else if (getCategory() == Category::PlayerSurvivor)
	{
		if (getVelocity().x != 0.f || getVelocity().y != 0.f) {
			mAction = MOVE;
			mIsMoving = true;
			mRifleMoveAnim.update(dt);
			//std::cout << "MOVING ANIM " << std::endl;
		}
		else {
			//mAction = IDLE;
			mIsMoving = false;
			mRifleMoveAnim.restart();
		}
		if (mIsReloading && mGunEquipped == 3) {
			//std::cout << "RELOAD! : ammo : " << mAmmo << std::endl;
			commands.push(mReloadCommand);
			mAction = RELOAD;
			mRifleReloadAnim.update(dt);
		}
		else {
			checkProjectileLaunch(dt, commands);
		}

		if (mRifleReloadAnim.isFinished() && mIsReloading) {
			mRifleReloadAnim.restart();
			mIsReloading = false;
			mAction = IDLE;
		}

		if (mAction == IDLE && mGunEquipped == 1) {
			mHandgunIdleAnim.update(dt);
		}
		else if (mAction == IDLE && mGunEquipped == 3) {
			mRifleIdleAnim.update(dt);
		}
		/*else if (mIsMoving) {
			mRifleMoveAnim.update(dt);
		}
		else {
			mRifleMoveAnim.restart();
		}*/

		//std::cout << "action: " << mAction << std::endl;
	}

	// check if bullets are fired

	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

}


bool Character::isMarkedForRemoval() const
{
	return isDestroyed() && (mBloodAnim.isFinished() || !mShowBlood);
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

	switch (mAction) {
	case SHOOT:
		/*mPlayerShoot.update(dt);*/
		break;
	case RELOAD:

		break;
	case MOVE:
		break;
	default:
		break;
	}
}

void Character::updateTexts()
{
	if (getCategory() == Category::PlayerSurvivor) {
		mPlayerHealth.setString(toString(getHitpoints()) + " HP");
		mPlayerHealth.setPosition(360.f, 4760.f);
		mPlayerHealth.setCharacterSize(10u);
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
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
		mAction = SHOOT;
		mAmmo--;
		//std::cout << "FIRING" << std::endl;

		mRifleShootAnim.update(dt);
		//updatePlayerAnimation(dt);
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
		//mAction = SHOOT;
		//std::cout << "FIRING" << std::endl;
	}
	else if (!mIsFiring && mAction != MOVE) {
		mAction = IDLE;
		//std::cout << "IDLE" << std::endl;
	}
}

void Character::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = mProjectileType;
	//Projectile::Type type = Projectile::HandgunBullet;
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

void Character::setHordeLevel(int hourdNum)
{
	mHordeLevel = hourdNum;
}


void Character::moveAim()
{
	setRotation(mDirectionAngle);
}

float Character::getMaxSpeed() const
{
	return Table[mType].speed;
}

Animation Character::getGunAnimationObj(int gun, int action) const
{
	if (action == IDLE) {
		if (gun == 1)
			return mHandgunIdleAnim;
		else if (gun == 2)
			return mShotgunIdleAnim;
		else if (gun == 3)
			return mRifleIdleAnim;
	}
	else if (action == SHOOT) {
		if (gun == 1)
			return mHandgunShootAnim;
		else if (gun == 2)
			return mShotgunShootAnim;
		else if (gun == 3)
			return mRifleShootAnim;
	}
	else if (action == MOVE) {
		if (gun == 1)
			return mHandgunMoveAnim;
		else if (gun == 2)
			return mShotgunMoveAnim;
		else if (gun == 3)
			return mRifleMoveAnim;

	}
	else if (action == RELOAD) {
		if (gun == 1)
			return mHandgunIdleAnim;
		else if (gun == 2)
			return mShotgunIdleAnim;
		else if (gun == 3)
			return mRifleReloadAnim;
	}
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

bool Character::isFiring() const
{
	return mAction == SHOOT;
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

void Character::chase()
{
	//mIsZombieChasing = true;
	mIsZombieAttacking = false;
}

void Character::attack()
{
	mIsZombieAttacking = true;
}

void Character::reload()
{
	if (mAmmo >= 60)
		return;

	mIsReloading = true;
	mAmmo = 60;
}

void Character::changeGun(int gunNum)
{
	if (gunNum == mGunEquipped)
		return;

	switch (gunNum)
	{
	case 1:
		std::cout << "HANDUNG CHOOSE" << std::endl;
		mProjectileType = Projectile::Type::HandgunBullet;
		mFireRateLevel = 5;
		mAmmo = 16;
		break;
	case 2:
		std::cout << "SHOTGUN CHOOSE" << std::endl;
		mProjectileType = Projectile::Type::ShotgunBullet;
		mFireRateLevel = 1;
		mAmmo = 6;
		break;
	case 3:
		std::cout << "RIFLE CHOOSE" << std::endl;
		mProjectileType = Projectile::Type::RifleBullet;
		mFireRateLevel = 10;
		mAmmo = 60;
		break;
		/*case 4:
			std::cout << "KNIFE CHOOSE" << std::endl;
			break;*/
	default:
		break;
	}

	mGunEquipped = gunNum;
}


