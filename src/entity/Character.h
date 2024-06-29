#pragma once
#ifndef SURVIVOR_H
#define SURVIVOR_H

#include "Entity.h"

class Character : public Entity
{
	public:
		enum Type {
			Survivor,
			Zombie
		};

	public:
		explicit Character(Type type);

	private:
		Type	mType;
};
#endif // !SURVIVOR_H

