#pragma once
#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "../../util/ResourceIdentifiers.h"
#include <vector>

struct CharacterData
{
	int				hitpoints;
	float			speed;
	Textures::ID	texture;
};

std::vector<CharacterData> initializeCharacterData();
#endif // !DATA_TABLES_H


