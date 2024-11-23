#pragma once
#ifndef SOUND_NODE_H
#define SOUND_NODE_H

#include <SFML/System/Vector2.hpp>

#include "SceneNode.h"
#include "../util/ResourceIdentifiers.h"
#include "../sound/SoundPlayer.h"

class SoundNode : public SceneNode
{
public:
	explicit					SoundNode(SoundPlayer& player);
	void					playSound(SoundEffect::ID sound, sf::Vector2f position);

	virtual unsigned int	getCategory() const;

private:
	SoundPlayer&			mSounds;
};
#endif // !SOUND_NODE_H

