#pragma once
#ifndef SURVIVOR_H
#define SURVIVOR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.h"
#include "../input/command/CommandQueue.h"
#include "../util/ResourceIdentifiers.h"
#include "../ecs/TextNode.h"
#include "Projectile.h"
#include "../graphics/Animation.h"

class Character : public Entity
{
	public:
		enum Type {
			Survivor,
			Zombie,
			TypeCount,
		};

		enum AnimationState {
			IDLE,
			MOVE,
			ATTACK,
			RELOAD,
			SHOOT
		};

		enum WeaponSlot {
			HANDGUN,
			SHOTGUN,
			RIFLE
		};

		struct WeaponData {
			int id;
			int currentAmmo;
			int maxAmmo;
			int totalAmmo;
			bool available; // tuve que hacer esto para mostrar el arma en caso que agarre el item, por pedazo de pelotudo y borrar todo el sistema
			// que hacia que agregue el objeto del arma a la lista del inventario, solucion rapida para no tener que hacer todo de nuevo
			// puto
			// puto el que lee
		};

	public:
		explicit Character(Type type, const TextureHolder& textures, const FontHolder& fonts);
        virtual unsigned int getCategory() const;
		bool	isAllied() const;

	public:
		void	setDirectionAngle(float angle);
		void	setMousePosition(sf::Vector2f mousePosition);
		float	getDirectionAngle() const;

		void setHordeLevel(int hourdNum);

		void moveAim();

		float getMaxSpeed() const;

		Animation getGunAnimationObj(int gun, int action) const;
		sf::FloatRect getBoundingRect() const;
		sf::Vector2f getGunPosition() const;
		int getCurrentAmmunition(int gun) const;
		int decrementCurrentAmmo(int gunType);
		virtual bool		isMarkedForRemoval() const;
		virtual void		remove();

		// zombie only methods
		void guideTowardsPlayer(sf::Vector2f position);
		bool isChasing() const;
		bool isFiring() const;

		void	increaseFireRate();
		void	increaseAmmo(int gun);
		void	splashBlood();
		void	fire();
		void	chase();
		void	attack();
		void	reload();
		void	changeGun(int gunNum);
		void	addGun(int gunNum);
		bool	isGunInInventory(int gunNum);

private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt, CommandQueue&);

		void updateMovementPattern(sf::Time dt);
		void updatePlayerAnimation(sf::Time dt, Animation& anim) const;
		void checkGunAnimation(sf::Time dt);

		void		 updateTexts();
		void		 checkPickupDrop(CommandQueue& commands);
		void		 checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

		void		 createBullets(SceneNode& node, const TextureHolder& textures) const;
		void		 createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
		void		 createPickup(SceneNode& node, const TextureHolder& textures) const;

private:
		Type			mType;
		sf::Sprite		mSprite;
		sf::IntRect		mTextureRect;
		Command			mFireCommand;
		Command			mReloadCommand;
		Command			mDropPickupCommand;
		Command			mChangeGunCommand;

		Projectile::Type mProjectileType;
		/*** Survivor animations **********/
		Animation			mHandgunIdleAnim;
		Animation			mHandgunMoveAnim;
		Animation			mHandgunShootAnim;
		//Animation			mHandgunReloadAnim;
		Animation			mRifleIdleAnim;
		Animation			mRifleMoveAnim;
		Animation			mRifleShootAnim;
		Animation			mRifleReloadAnim;
		Animation			mShotgunIdleAnim;
		Animation			mShotgunMoveAnim;
		Animation			mShotgunShootAnim;
		//Animation			mShotgunReloadAnim;
		/*********************************/
		/*** Zombie Animations ************/
		Animation		mZombieIdleAnim;
		Animation		mZombieMoveAnim;
		Animation		mZombieAttackAnim;
		/*********************************/
		Animation		mBloodAnim;

		sf::Vector2f	mCenter;
		float			mDirectionAngle;
		float			mTravelledDistance;

		int				mDirectionIndex;
		int				mFireRateLevel;
		int				mSpreadLevel;
		int				mLeftTexture;
		int				mWidthTexture;
		int				mHeightTexture;
		int				mCurrentAmmo;
		int				mMaxAmmo;
		int				mAction;
		int				mAmmoFired;
		int				mHordeLevel;
		int				mGunEquipped;
		std::vector<std::unique_ptr<WeaponData>>	mGunInventoryList;
		std::vector<std::unique_ptr<WeaponData>>	mGunAvailableList;

		TextNode*		mHealthDisplay;
		sf::Time		mFireCountdown;
		sf::Time		mElapsedFrameTime;
		sf::Vector2f	mZombieTargetDirection;
		sf::Vector2f	mMousePosition;
		sf::Vector2f	mGunPosition;
		sf::Text		mPlayerHealth;
		std::size_t		mCurrentFrame;

		bool			mIsMoving;
		bool			mIsFiring;
		bool			mIsReloading;
		bool			mIsMarkedForRemoval;
		bool			mShowBlood;
		bool			mSpawnedPickup;
		bool			mIsZombieAttacking;
		bool			mIsZombieChasing;
};
#endif // !SURVIVOR_H

