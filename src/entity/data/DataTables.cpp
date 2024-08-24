#include "DataTables.h"
#include "../Character.h"
#include "../Projectile.h"
#include "../Pickup.h"

using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::TypeCount);

	data[Character::Survivor].hitpoints = 100;
	data[Character::Survivor].speed = 300.f;
	data[Character::Survivor].fireInterval = sf::seconds(1);
	data[Character::Survivor].texture = Textures::Survivor;

	data[Character::Zombie].hitpoints = 20;
	data[Character::Zombie].speed = 80.f;
	data[Character::Zombie].texture = Textures::Zombie;
	/*  data[Character::Zombie].directions.push_back(Direction(45, 80));*/
	  //data[Character::Zombie].directions.push_back(Direction(-45, 160));
	  //data[Character::Zombie].directions.push_back(Direction(45, 80));



	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::HandgunBullet].damage = 10;
	data[Projectile::HandgunBullet].speed = 1000.f;
	data[Projectile::HandgunBullet].texture = Textures::HandgunBullet;

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action = [](Character& c) {c.heal(25);  };

	data[Pickup::FireSpread].texture = Textures::FireSpread;
	data[Pickup::HealthRefill].action = std::bind(&Character::increaseSpread, _1);

	data[Pickup::FireRate].texture = Textures::FireRate;
	data[Pickup::FireRate].action = std::bind(&Character::increaseFireRate, _1);

	return data;
}
