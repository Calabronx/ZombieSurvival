#pragma once
#ifndef SURVIVOR_H
#define SURVIVOR_H

#include <SFML/Graphics.hpp>

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

	private:
		Type			mType;
		sf::Sprite		mSprite;
};
#endif // !SURVIVOR_H

