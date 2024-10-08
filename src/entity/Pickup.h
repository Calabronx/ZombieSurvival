#pragma once
#ifndef PICKUP_H
#define PICKUP_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.h"
#include "Character.h"
#include "../util/ResourceIdentifiers.h"

class Pickup : public Entity
{
public:
	enum Type
	{
		HealthRefill,
		HandgunAmmo,
		ShotgunAmmo,
		RifleAmmo,
		FireRate,
		ShotgunItem,
		RifleItem,
		TypeCount
	};

public:
	Pickup(Type type, const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void apply(Character& player) const;

protected:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
};
#endif 

