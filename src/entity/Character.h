#pragma once
#ifndef SURVIVOR_H
#define SURVIVOR_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Entity.h"
#include "../util/ResourceIdentifiers.h"

class Character : public Entity
{
	public:
		enum Type {
			Survivor,
			Zombie
		};

	public:
		explicit Character(Type type, const TextureHolder& textures);
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual unsigned int getCategory() const;

public:
	void setDirectionAngle(float angle);
	void moveAim();


private:
		Type			mType;
		sf::Sprite		mSprite;
		sf::Vector2f	mCenter;
		float			mDirectionAngle;
};
#endif // !SURVIVOR_H

