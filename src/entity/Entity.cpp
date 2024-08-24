#include "Entity.h"

Entity::Entity(int hitpoints)
	: mHitpoints(hitpoints)
	, mEntityStop(false)
{
}

void Entity::damage(int points)
{
}

void Entity::destroy()
{
}

int Entity::getHitpoints() const
{
	return mHitpoints;
}

bool Entity::isDestroyed() const
{
	return mHitpoints <= 0;
}

void Entity::heal(int points)
{
	assert(points > 0);

	mHitpoints += points;
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::stop(bool isStop)
{
	mEntityStop = isStop;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::moveEntity(sf::Vector2f velocity)
{
	if(!mEntityStop)
		mVelocity += velocity;
}

void Entity::moveEntity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	move(mVelocity * dt.asSeconds());
}

