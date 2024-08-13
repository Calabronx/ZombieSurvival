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
    //data[Character::Zombie].directions.push_back(Direction(45, 80));
    //data[Character::Zombie].directions.push_back(Direction(-45, 160));
    //data[Character::Zombie].directions.push_back(Direction(45, 80));



    return data;
}
