#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Entity.h"
#include "../util/ResourceIdentifiers.h"
#include "../input/command/CommandQueue.h"

class Projectile : public Entity
{
	public:
		enum Type
		{
			//HandgunBullet
			//ShotgunBullet
			HandgunBullet,
			ShotgunBullet,
			TypeCount
		};

	public:
		Projectile(Type type, const TextureHolder& textures);

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		float					getMaxSpeed() const;
		int						getDamage();

	private:
		virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		Type			mType;
		sf::Sprite		mSprite;
		sf::Vector2f	mTargetDirection;
};
#endif

