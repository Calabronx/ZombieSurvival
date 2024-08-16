#pragma once
#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "../../util/ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
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
	sf::Time				fireInterval;
	std::vector<Direction>	directions;
};

struct ProjectileData
{
	int				damage;
	float			speed;
	Textures::ID	texture;
};

std::vector<CharacterData> initializeCharacterData();
std::vector<ProjectileData> initializeProjectileData();
#endif // !DATA_TABLES_H


