#pragma once
#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "../../util/ResourceIdentifiers.h"
#include <vector>

struct Direction
{
	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{
	}

	float angle;
	float distance;
};

struct CharacterData
{
	int						hitpoints;
	float					speed;
	Textures::ID			texture;
	std::vector<Direction>	directions;
};

std::vector<CharacterData> initializeCharacterData();
#endif // !DATA_TABLES_H


