#include "Character.h"
#include "data/DataTables.h"
#include "../util/FileSystem.h"
#include "../util/Category.h"
#include "../util/Utility.h"
#include "Pickup.h"
#include <iostream>
#include "../graphics/EmitterNode.h"
#include "../ecs/SoundNode.h"

using namespace std::placeholders;

namespace {
	const std::vector<CharacterData> Table = initializeCharacterData();
	const std::vector<WeaponData> WeaponDataTable = initializeWeaponData();
}

Character::Character(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mBloodAnim(textures.get(Textures::Blood))
	, mHealthDisplay(nullptr)
	, mAmmoDisplay(nullptr)
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
	, mCurrentAmmo(16)
	, mBulletLaunchPosition()
	//, mPlayerHealth()
	, mGunInventoryList(3)
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
	mBloodAnim.setPosition(mBloodAnim.getPosition() + sf::Vector2f(+60.f, +0.0f));

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	std::unique_ptr<TextNode> ammoDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	mAmmoDisplay = ammoDisplay.get();

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
		mHandgunShootAnim.setFrameSize(sf::Vector2i(250, 210));
		mHandgunShootAnim.setNumFrames(3);
		mHandgunShootAnim.setDuration(sf::seconds(0.0001));
		mHandgunShootAnim.setTexture(textures.get(Textures::HandgunShoot));
		mHandgunShootAnim.setRepeating(true);
		//mHandgunShootAnim.setPosition(mHandgunShootAnim.getPosition() - sf::Vector2f(0.f, -50.f));
		centerOrigin(mHandgunShootAnim);
		/*	mHandgunReloadAnim.setFrameSize(sf::Vector2i(330, 220));
			mHandgunReloadAnim.setNumFrames(14);
			mHandgunReloadAnim.setDuration(sf::seconds(2));
			mHandgunReloadAnim.setTexture(textures.get(Textures::HandgunReload));
			mHandgunReloadAnim.setRepeating(false);
			centerOrigin(mHandgunReloadAnim);*/
		mHandgunMoveAnim.setFrameSize(sf::Vector2i(260, 220));
		mHandgunMoveAnim.setNumFrames(18);
		mHandgunMoveAnim.setDuration(sf::seconds(1));
		mHandgunMoveAnim.setTexture(textures.get(Textures::HandgunMove));
		mHandgunMoveAnim.setRepeating(true);
		centerOrigin(mHandgunMoveAnim);
		// Shotgun animations
		mShotgunShootAnim.setFrameSize(sf::Vector2i(310, 200));
		mShotgunShootAnim.setNumFrames(3);
		mShotgunShootAnim.setDuration(sf::seconds(0.0005));
		mShotgunShootAnim.setTexture(textures.get(Textures::ShotgunShoot));
		mShotgunShootAnim.setRepeating(true);
		//mShotgunShootAnim.setPosition(mShotgunShootAnim.getPosition() - sf::Vector2f(0.f, -50.f));
		centerOrigin(mShotgunShootAnim);
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
		centerOrigin(mShotgunMoveAnim);

		mShootFireAnim1.setFrameSize(sf::Vector2i(64, 64));
		mShootFireAnim1.setNumFrames(16);
		mShootFireAnim1.setTexture(textures.get(Textures::ShootFire));
		mShootFireAnim1.setDuration(sf::seconds(0.5000));
		mShootFireAnim1.setScale(sf::Vector2f(4.5f, 1.5f));
		mShootFireAnim1.setRepeating(false);
		centerOrigin(mShootFireAnim1);
		mShootFireAnim1.setPosition(mShootFireAnim1.getPosition() + sf::Vector2f(50.f, 40.0f));

		mShootFireAnim2.setFrameSize(sf::Vector2i(64, 64));
		mShootFireAnim2.setNumFrames(16);
		mShootFireAnim2.setTexture(textures.get(Textures::ShootFire));
		mShootFireAnim2.setDuration(sf::seconds(0.5000));
		mShootFireAnim2.setScale(sf::Vector2f(4.5f, 3.5f));
		mShootFireAnim2.setRepeating(false);
		centerOrigin(mShootFireAnim2);
		mShootFireAnim2.setPosition(mShootFireAnim2.getPosition() + sf::Vector2f(50.f, 10.0f));

		mShootFireAnim3.setFrameSize(sf::Vector2i(64, 64));
		mShootFireAnim3.setNumFrames(16);
		mShootFireAnim3.setTexture(textures.get(Textures::ShootFire));
		mShootFireAnim3.setDuration(sf::seconds(0.5000));
		mShootFireAnim3.setScale(sf::Vector2f(4.5f, 1.5f));
		mShootFireAnim3.setRepeating(false);
		centerOrigin(mShootFireAnim3);
		mShootFireAnim3.setPosition(mShootFireAnim3.getPosition() + sf::Vector2f(50.f, -10.0f));


		mIsFiring = false;
		mIsReloading = false;
		mIsMoving = false;
		mFireCountdown = sf::Time::Zero;
		mFireRateLevel = 1;
		mDirectionAngle = 0.0f;
		mAction = IDLE;
		mProjectileType = Projectile::Type::HandgunBullet;
		mGunEquipped = 1;
		//addGun(1);

		std::unique_ptr<WeaponData> handgun(new WeaponData());
		std::unique_ptr<WeaponData> shotgun(new WeaponData());
		std::unique_ptr<WeaponData> rifle(new WeaponData());

		handgun->id = WeaponDataTable[HANDGUN].id;
		handgun->currentAmmo = WeaponDataTable[HANDGUN].currentAmmo;
		handgun->maxAmmo = WeaponDataTable[HANDGUN].maxAmmo;
		handgun->totalAmmo = WeaponDataTable[HANDGUN].totalAmmo;
		handgun->available = WeaponDataTable[HANDGUN].available;
		mGunInventoryList[HANDGUN] = std::move(handgun);

		shotgun->id = WeaponDataTable[SHOTGUN].id;
		shotgun->currentAmmo = WeaponDataTable[SHOTGUN].currentAmmo;
		shotgun->maxAmmo = WeaponDataTable[SHOTGUN].maxAmmo;
		shotgun->totalAmmo = WeaponDataTable[SHOTGUN].totalAmmo;
		shotgun->available = false;
		mGunInventoryList[SHOTGUN] = std::move(shotgun);

		rifle->id = WeaponDataTable[RIFLE].id;
		rifle->currentAmmo = WeaponDataTable[RIFLE].currentAmmo;
		rifle->maxAmmo = WeaponDataTable[RIFLE].maxAmmo;
		rifle->totalAmmo = WeaponDataTable[RIFLE].totalAmmo;
		rifle->available = false;
		mGunInventoryList[RIFLE] = std::move(rifle);

		mTotalAmmo = mGunInventoryList[HANDGUN]->totalAmmo;
		attachChild(std::move(healthDisplay));
		attachChild(std::move(ammoDisplay));

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
	if (isDestroyed() && mShowBlood) {
		target.draw(mBloodAnim, states);
	}
	else if (getCategory() == Category::PlayerSurvivor) {
		target.draw(getGunAnimation(mGunEquipped, mAction), states);
		if (mAction == SHOOT) {
			target.draw(mShootFireAnim1, states);
			target.draw(mShootFireAnim2, states);
			target.draw(mShootFireAnim3, states);
		}
	}
	else if (getCategory() == Category::Zombie) {
		if (mAction == MOVE)
			target.draw(mZombieMoveAnim, states);
		else if (mAction == ATTACK)
			target.draw(mZombieAttackAnim, states);
	}
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
			sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mZombieTargetDirection + getVelocity()); // me parece que esta calculando con parametro 0 de vector
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
			checkGunAnimation(dt);
			//playLocalSound(commands, SoundEffect::Step1); se escucha uno tras otro y suena mal          

		}
		else {
			mIsMoving = false;
		}

		if (mIsReloading) {
			//std::cout << "RELOAD! : ammo : " << mAmmo << std::endl;
			
			mAction = RELOAD;
			checkGunAnimation(dt);
		}
		else {
			checkProjectileLaunch(dt, commands);
		}

		if (mGunEquipped == 3 && mRifleReloadAnim.isFinished()) {
			mRifleReloadAnim.restart();
			mIsReloading = false;
			mAction = IDLE;
		}
		else if (mGunEquipped == 1 && mIsReloading || mGunEquipped == 2 && mIsReloading) {
			mIsReloading = false;
			mAction = IDLE;
			//playLocalSound(commands, SoundEffect::HandgunReload);
		}

		checkGunAnimation(dt);
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

void Character::updatePlayerAnimation(sf::Time dt, Animation& anim) const
{
	anim.update(dt);
}

void Character::checkGunAnimation(sf::Time dt)
{
	if (mAction == IDLE) {
		if (mGunEquipped == 1) {
			updatePlayerAnimation(dt, mHandgunIdleAnim);
		}
		else if (mGunEquipped == 2) {
			updatePlayerAnimation(dt, mShotgunIdleAnim);
		}
		else if (mGunEquipped == 3) {
			updatePlayerAnimation(dt, mRifleIdleAnim);
		}
	}
	else if (mAction == SHOOT) {
		if (mGunEquipped == 1) {
			updatePlayerAnimation(dt, mHandgunShootAnim);
		}
		else if (mGunEquipped == 2) {
			updatePlayerAnimation(dt, mShotgunMoveAnim);
		}
		else if (mGunEquipped == 3) {
			updatePlayerAnimation(dt, mRifleMoveAnim);
		}
	}
	else if (mAction == MOVE) {
		if (mGunEquipped == 1) {
			updatePlayerAnimation(dt, mHandgunMoveAnim);
		}
		else if (mGunEquipped == 2) {
			updatePlayerAnimation(dt, mShotgunMoveAnim);
		}
		else if (mGunEquipped == 3) {
			updatePlayerAnimation(dt, mRifleMoveAnim);
		}
	}
	else if (mAction == RELOAD) {
		if (mGunEquipped == 1) {
			updatePlayerAnimation(dt, mHandgunIdleAnim);
		}
		else if (mGunEquipped == 2) {
			updatePlayerAnimation(dt, mShotgunIdleAnim);
		}
		else if (mGunEquipped == 3) {
			updatePlayerAnimation(dt, mRifleReloadAnim);
		}
	}
}

void Character::updateTexts()
{
	if (getCategory() == Category::PlayerSurvivor) {
		/*mHealthDisplay->setString(toString(getHitpoints()) + " HP");
		mHealthDisplay->setPosition(0.f, 95.f);
		mHealthDisplay->setRotation(-getRotation());

		mAmmoDisplay->setString("AMMO: " + toString(mCurrentAmmo));
		mAmmoDisplay->setPosition(0.f, 150.f);
		mAmmoDisplay->setRotation(-getRotation());*/
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
		playLocalSound(commands, SoundEffect::Gunfire);

		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mShootFireAnim1.update(dt);
		mShootFireAnim2.update(dt);
		mShootFireAnim3.update(dt);

		std::unique_ptr<EmitterNode> smoke1(new EmitterNode(Particle::Smoke));
		std::unique_ptr<EmitterNode> smoke2(new EmitterNode(Particle::Smoke));
		std::unique_ptr<EmitterNode> smoke3(new EmitterNode(Particle::Smoke));
		smoke1->setPosition(mShootFireAnim1.getPosition() + sf::Vector2f(100.f,100.f));
		smoke2->setPosition(mShootFireAnim2.getPosition() + sf::Vector2f(100.f, 100.f));
		smoke3->setPosition(mShootFireAnim3.getPosition() + sf::Vector2f(100.f, 100.f));
		attachChild(std::move(smoke1));
		attachChild(std::move(smoke2));
		attachChild(std::move(smoke3));

		mIsFiring = false;
		mAction = SHOOT;
		decrementCurrentAmmo(mGunEquipped);
		//std::cout << "FIRING" << std::endl;

		if (mGunEquipped == 3)
			mRifleShootAnim.update(dt);
		else if (mGunEquipped == 2)
			mShotgunShootAnim.update(dt);
		else if (mGunEquipped == 1)
			mHandgunShootAnim.update(dt);

		checkGunAnimation(dt);
		//updatePlayerAnimation(dt);
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
		mShootFireAnim1.restart();
		mShootFireAnim2.restart();
		mShootFireAnim3.restart();
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

	if (mGunEquipped == 2) {
		createProjectile(node, type, -0.2f, 0.0f, textures);
		createProjectile(node, type, 0.2f, 0.0f, textures);
		createProjectile(node, type, -0.4f, 0.0f, textures);
		createProjectile(node, type, 1.0f, 0.0f, textures);
		createProjectile(node, type, 0.4f, 0.0f, textures);
		createProjectile(node, type, -0.8f, 0.0f, textures);
	}
	else {
		createProjectile(node, type, xOffset, yOffset, textures);

	}
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
	if (type == Pickup::RifleItem || type == Pickup::ShotgunItem)  // evitar el spwan de items de arma
		return;

	/*if (type == Pickup::RifleAmmo && mGunInventoryList[RIFLE]->totalAmmo >= 60 || type == Pickup::ShotgunAmmo 
		&& mGunInventoryList[SHOTGUN]->totalAmmo >= 12)
		return;*/

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 0.f);
	node.attachChild(std::move(pickup));
}

void Character::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, _1, effect, getWorldPosition()));

	commands.push(command);
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

Animation Character::getGunAnimation(int gun, int action) const
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

int Character::getCurrentAmmunition() const
{
	return mCurrentAmmo;
}

int Character::getCurrentTotalAmmunition() const
{
	return mTotalAmmo;
}

int Character::decrementCurrentAmmo(int gunType)
{
	for (auto i = 0; i < mGunInventoryList.size(); i++) {
		if (mGunInventoryList[i]->id == gunType && mCurrentAmmo <= mGunInventoryList[i]->maxAmmo) {
			mCurrentAmmo = --mGunInventoryList[i]->currentAmmo;
		}
	}
	std::cout << "AMMO: " << mCurrentAmmo << std::endl;
	return 0;
}

void Character::guideTowardsPlayer(sf::Vector2f position)
{
	if (mType != Type::Zombie)
	{
		return;
	}

	mZombieTargetDirection = unitVector(position - getWorldPosition()); // revisar valor parametro
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


void Character::increaseAmmo(int gun)
{
	if(gun == 1)
		mGunInventoryList[HANDGUN]->totalAmmo += 30;
	else if (gun == 2)
		mGunInventoryList[SHOTGUN]->totalAmmo += 8;
	else if (gun == 3)
		mGunInventoryList[RIFLE]->totalAmmo += 60;
}
void Character::splashBlood(sf::Vector2f impactPos)
{
	std::unique_ptr<EmitterNode> blood(new EmitterNode(Particle::Blood));
	blood->setPosition(impactPos);
	attachChild(std::move(blood));
}
void Character::fire()
{
	if (mCurrentAmmo <= 0)
		return;


	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;

	//std::cout << "fire" << std::endl;
}

void Character::chase()
{
	mIsZombieAttacking = false;
}

void Character::attack()
{
	mIsZombieAttacking = true;
}

void Character::reload()
{
	if (mGunEquipped == 1) {
		if (mGunInventoryList[HANDGUN]->totalAmmo <= 0) 
			return;
		
		mGunInventoryList[HANDGUN]->currentAmmo = WeaponDataTable[HANDGUN].maxAmmo;
		mGunInventoryList[HANDGUN]->totalAmmo -= (mCurrentAmmo - mGunInventoryList[HANDGUN]->currentAmmo) * -1;
		mCurrentAmmo = mGunInventoryList[HANDGUN]->currentAmmo;
		mTotalAmmo = mGunInventoryList[HANDGUN]->totalAmmo;
		//playLocalSound()
	}
	else if (mGunEquipped == 2) {
		if (mGunInventoryList[SHOTGUN]->totalAmmo <= 0)
			return;

		mGunInventoryList[SHOTGUN]->currentAmmo = WeaponDataTable[SHOTGUN].maxAmmo;
		mGunInventoryList[SHOTGUN]->totalAmmo -= (mCurrentAmmo - mGunInventoryList[SHOTGUN]->currentAmmo) * -1;
		mCurrentAmmo = mGunInventoryList[SHOTGUN]->currentAmmo;
		mTotalAmmo = mGunInventoryList[SHOTGUN]->totalAmmo;

	}
	else if (mGunEquipped == 3) {
		if (mGunInventoryList[RIFLE]->totalAmmo <= 0)
			return;

		mGunInventoryList[RIFLE]->currentAmmo = WeaponDataTable[RIFLE].maxAmmo;
		mGunInventoryList[RIFLE]->totalAmmo -= (mCurrentAmmo - mGunInventoryList[RIFLE]->currentAmmo) * -1;
		mCurrentAmmo = mGunInventoryList[RIFLE]->currentAmmo;
		mTotalAmmo = mGunInventoryList[RIFLE]->totalAmmo;

	}
	else {
		return;
	}

	mIsReloading = true;
}

void Character::changeGun(int gunNum)
{
	if (gunNum == mGunEquipped)
		return;

	if (!isGunInInventory(gunNum)) 
		return;
	

	switch (gunNum)
	{
	case 1:
		mProjectileType = Projectile::Type::HandgunBullet;
		mFireRateLevel = 8;
		mCurrentAmmo = mGunInventoryList[HANDGUN]->currentAmmo;
		mTotalAmmo = mGunInventoryList[HANDGUN]->totalAmmo;
		mShootFireAnim1.setScale(sf::Vector2f(4.5f, 1.5f));		
		mShootFireAnim2.setScale(sf::Vector2f(4.5f, 1.5f));		
		mShootFireAnim3.setScale(sf::Vector2f(4.5f, 1.5f));		
		break;
	case 2:
		mProjectileType = Projectile::Type::ShotgunBullet;
		mFireRateLevel = 1;
		mCurrentAmmo = mGunInventoryList[SHOTGUN]->currentAmmo;
		mTotalAmmo = mGunInventoryList[SHOTGUN]->totalAmmo;
		mShootFireAnim1.setScale(sf::Vector2f(4.5f, 3.5f));
		mShootFireAnim2.setScale(sf::Vector2f(5.5f, 3.5f));
		mShootFireAnim3.setScale(sf::Vector2f(4.5f, 3.5f));
		break;
	case 3:
		mProjectileType = Projectile::Type::RifleBullet;
		mFireRateLevel = 10;
		mCurrentAmmo = mGunInventoryList[RIFLE]->currentAmmo;
		mTotalAmmo = mGunInventoryList[RIFLE]->totalAmmo;
		mShootFireAnim1.setScale(sf::Vector2f(2.5f, 3.5f));
		mShootFireAnim2.setScale(sf::Vector2f(4.5f, 3.5f));
		mShootFireAnim3.setScale(sf::Vector2f(2.5f, 3.5f));
		break;
		/*case 4:
			std::cout << "KNIFE CHOOSE" << std::endl;
			break;*/
	default:
		return;
	}

	mGunEquipped = gunNum;
}

void Character::addGun(int gunNum)
{
	for (auto i = 0; i < mGunInventoryList.size(); i++) {
		if (mGunInventoryList[i]->id == gunNum)
			mGunInventoryList[i]->available = true;
	}
}

bool Character::isGunInInventory(int gunNum)
{
	for (auto i = 0; i < mGunInventoryList.size(); i++) {
		if (gunNum == mGunInventoryList[i]->id && !mGunInventoryList[i]->available) {
			return false;
		}
	}
	return true;
}


