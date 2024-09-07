#pragma once
#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "../../util/ResourceIdentifiers.h"
#include "../Character.h"

#include <SFML/System/Time.hpp>
#include <vector>
#include <functional>

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
	std::vector<Textures::ID> textureFrames;
	sf::IntRect				textureRect;
	sf::Time				fireInterval;
	std::vector<Direction>	directions;
	//bool					hasRollAnimation;
};

struct ProjectileData
{
	int				damage;
	float			speed;
	Textures::ID	texture;
	sf::IntRect		textureRect;
};

struct PickupData
{
	std::function<void(Character&)> action;
	Textures::ID							texture;
	sf::IntRect										textureRect;
};

std::vector<CharacterData> initializeCharacterData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
#endif // !DATA_TABLES_H


