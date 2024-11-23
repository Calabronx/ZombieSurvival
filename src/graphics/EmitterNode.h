#pragma once
#ifndef EMITTERNODE_H
#define EMITTERNODE_H

#include "../ecs/SceneNode.h"
#include "Particle.h"
#include "ParticleNode.h"

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
	explicit		EmitterNode(Particle::Type type);


private:
	virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual bool		isDestroyed() const;

	void			emitParticles(sf::Time dt);

private:
	sf::Time		mAccumulatedTime;
	sf::Time		mEmissionTime;
	Particle::Type	mType;
	ParticleNode*	mParticleSystem;
	bool			mFinishEmission;

};

#endif // !EMITTERNODE_H
