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

public:
	void setDirectionAngle(float angle);
	void moveAim();

	sf::FloatRect getBoundingRect() const;

private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void updateCurrent(sf::Time dt, CommandQueue&);

		void		 updateTexts();


private:
		Type			mType;
		sf::Sprite		mSprite;
		sf::Vector2f	mCenter;
		float			mDirectionAngle;

		TextNode*		mHealthDisplay;
};
#endif // !SURVIVOR_H

