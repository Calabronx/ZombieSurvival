#pragma once
#ifndef SURVIVOR_H
#define SURVIVOR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
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

	public:
		explicit Character(Type type, const TextureHolder& textures, const FontHolder& fonts);
        virtual unsigned int getCategory() const;
		bool	isAllied() const;

public:
	void	setDirectionAngle(float angle);
	void	setMousePosition(sf::Vector2f mousePosition);
	float	getDirectionAngle() const;

	

	void moveAim();

	float getMaxSpeed() const;

	sf::FloatRect getBoundingRect() const;
	sf::Vector2f getGunPosition() const;
	virtual bool		isMarkedForRemoval() const;
	virtual void		remove();

	// zombie only methods
	void guideTowardsPlayer(sf::Vector2f position);
	bool isChasing() const;

	void	increaseFireRate();
	void	increaseSpread();
	void	splashBlood(sf::Time dt);
	void fire();

private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt, CommandQueue&);

		void updateMovementPattern(sf::Time dt);

		void		 updateTexts();
		void		 checkPickupDrop(CommandQueue& commands);
		void		 checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

		void		 createBullets(SceneNode& node, const TextureHolder& textures) const;
		void		 createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
		void		 createPickup(SceneNode& node, const TextureHolder& textures) const;

private:
		Type			mType;
		sf::Sprite		mSprite;
		Command			mFireCommand;
		Command			mDropPickupCommand;
		Animation		mBlood;
		Animation		mZombieAnim;
		//const TextureHolder&	mTextureFrames;
		sf::Vector2f	mCenter;
		float			mDirectionAngle;
		float			mTravelledDistance;

		int				mDirectionIndex;
		int				mFireRateLevel;
		int				mSpreadLevel;

		TextNode*		mHealthDisplay;
		sf::Vector2f	mZombieTargetDirection;
		sf::Time		mFireCountdown;
		sf::Vector2f	mMousePosition;
		sf::Vector2f	mGunPosition;

		bool			mIsFiring;
		bool			mIsMarkedForRemoval;
		bool			mShowBlood;
		bool			mSpawnedPickup;
};
#endif // !SURVIVOR_H

