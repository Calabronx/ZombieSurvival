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
	data[Character::Survivor].fireInterval = sf::seconds(0.5);
	data[Character::Survivor].texture = Textures::Survivor;
	//data[Character::Survivor].textureRect = sf::IntRect(0, 0, 48, 64);

	data[Character::Zombie].hitpoints = 100;
	data[Character::Zombie].speed = 80.f;
	data[Character::Zombie].texture = Textures::ZombieWalk1;
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk1);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk2);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk3);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk4);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk5);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk6);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk7);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk8);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk9);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk10);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk11);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk12);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk13);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk14);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk15);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk16);
	data[Character::Zombie].textureFrames.push_back(Textures::ZombieWalk17);
	//data[Character::Zombie].texture = Textures::ZombieWalk2;
	//data[Character::Zombie].texture = Textures::ZombieWalk3;
	//data[Character::Zombie].texture = Textures::ZombieWalk4;
	//data[Character::Zombie].texture = Textures::ZombieWalk5;
	//data[Character::Zombie].texture = Textures::ZombieWalk6;
	//data[Character::Zombie].texture = Textures::ZombieWalk7;
	//data[Character::Zombie].texture = Textures::ZombieWalk8;
	//data[Character::Zombie].texture = Textures::ZombieWalk9;
	//data[Character::Zombie].texture = Textures::ZombieWalk10;
	//data[Character::Zombie].texture = Textures::ZombieWalk11;
	//data[Character::Zombie].texture = Textures::ZombieWalk12;
	//data[Character::Zombie].texture = Textures::ZombieWalk13;
	//data[Character::Zombie].texture = Textures::ZombieWalk14;
	//data[Character::Zombie].texture = Textures::ZombieWalk15;
	//data[Character::Zombie].texture = Textures::ZombieWalk16;
	//data[Character::Zombie].texture = Textures::ZombieWalk17;
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
