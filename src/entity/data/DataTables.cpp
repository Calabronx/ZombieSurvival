#include "DataTables.h"
#include "../Character.h"
#include "../Projectile.h"
#include "../Pickup.h"

using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::TypeCount);

	data[Character::Survivor].hitpoints = 500;
	data[Character::Survivor].speed = 30.f;
	data[Character::Survivor].fireInterval = sf::seconds(0.150);
	data[Character::Survivor].texture = Textures::Survivor;
	//data[Character::Survivor].textureRect = sf::IntRect(0, 0, 48, 64);

	data[Character::Zombie].hitpoints = 100;
	data[Character::Zombie].speed = 80.f;
	data[Character::Zombie].texture = Textures::ZombieWalk;
	data[Character::Zombie].textureRect = sf::IntRect(0, 0, 288, 300);
	
	/*  data[Character::Zombie].directions.push_back(Direction(45, 80));*/
	  //data[Character::Zombie].directions.push_back(Direction(-45, 160));
	  //data[Character::Zombie].directions.push_back(Direction(45, 80));



	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::HandgunBullet].damage = 10;
	data[Projectile::HandgunBullet].speed = 1500.f;
	data[Projectile::HandgunBullet].texture = Textures::HandgunBullet;

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action = [](Character& c) {c.heal(25);  };

	data[Pickup::FireSpread].texture = Textures::FireSpread;
	data[Pickup::FireSpread].action = std::bind(&Character::increaseSpread, _1);

	data[Pickup::FireRate].texture = Textures::FireRate;
	data[Pickup::FireRate].action = std::bind(&Character::increaseFireRate, _1);

	return data;
}
