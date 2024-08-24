#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Vector2.hpp>
#include "../ecs/SceneNode.h"
#include "../input/command/CommandQueue.h"

class Entity : public SceneNode
{
	public:
			explicit	Entity(int hitpoints);
			void		damage(int points);
			void		destroy();

			int			getHitpoints() const;

			void		heal(int points);


			void		setVelocity(sf::Vector2f velocity);
			void		setVelocity(float vx, float vy);
			void		stop(bool isStop);
			sf::Vector2f getVelocity() const;
			void		moveEntity(sf::Vector2f velocity);
			void		moveEntity(float vx, float vy);
			virtual bool		isDestroyed() const;

	private:

	protected:
			virtual void updateCurrent(sf::Time dt, CommandQueue&);

	private:
			sf::Vector2f	mVelocity;
			int				mHitpoints;
			bool			mEntityStop;

};
#endif // !ENTITY_H

