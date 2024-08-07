#include "DataTables.h"
#include "../Character.h"

std::vector<CharacterData> initializeCharacterData()
{
    std::vector<CharacterData> data(Character::TypeCount);

    data[Character::Survivor].hitpoints = 100;
    data[Character::Survivor].speed = 200.f;
    data[Character::Survivor].texture = Textures::Survivor;

    data[Character::Zombie].hitpoints = 20;
    data[Character::Zombie].speed = 80.f;
    data[Character::Zombie].texture = Textures::Zombie;


    return data;
}
