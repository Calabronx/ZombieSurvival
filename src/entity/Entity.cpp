#include "Entity.h"

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

void Entity::updateCurrent(sf::Time dt)
{
	move(mVelocity * dt.asSeconds());
}
