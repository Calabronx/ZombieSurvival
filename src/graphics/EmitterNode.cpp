#include "EmitterNode.h"
#include "../util/Category.h"

EmitterNode::EmitterNode(Particle::Type type)
	: SceneNode()
	, mAccumulatedTime(sf::Time::Zero)
	, mEmissionTime(sf::Time::Zero)
	, mType(type)
	, mParticleSystem(nullptr)
	, mFinishEmission(false)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (mParticleSystem)
	{
		emitParticles(dt);
	}
	else
	{
		auto finder = [this](ParticleNode& container, sf::Time)
		{
			if (container.getParticleType() == mType)
				mParticleSystem = &container;
		};

		Command command;
		command.category = Category::ParticleSystem;
		command.action = derivedAction<ParticleNode>(finder);

		commands.push(command);
	}
}

bool EmitterNode::isDestroyed() const
{
	return mFinishEmission;
}

void EmitterNode::emitParticles(sf::Time dt)
{
	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	mAccumulatedTime += dt;
	mEmissionTime += dt;

	if (mEmissionTime >= mParticleSystem->getEmmisionTime())
		mFinishEmission = true;
	else
		while (mAccumulatedTime > interval)
		{
			mAccumulatedTime -= interval;
			mParticleSystem->addParticle(getWorldPosition());
		}
}
