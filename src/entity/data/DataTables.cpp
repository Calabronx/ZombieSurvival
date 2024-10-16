#include "DataTables.h"
#include "../Character.h"
#include "../Projectile.h"
#include "../Pickup.h"
#include "../../graphics/Particle.h"

using namespace std::placeholders;

std::vector<CharacterData> initializeCharacterData()
{
	std::vector<CharacterData> data(Character::TypeCount);

	//data[Character::Survivor].hitpoints = 500;
	data[Character::Survivor].hitpoints = 1500;
	data[Character::Survivor].speed = 2.f;
	data[Character::Survivor].fireInterval = sf::seconds(1);
	data[Character::Survivor].texture = Textures::Survivor;
	//data[Character::Survivor].textureRect = sf::IntRect(0, 0, 48, 64);

	data[Character::Zombie].hitpoints = 300;
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
	data[Projectile::RifleBullet].damage = 10;
	data[Projectile::RifleBullet].speed = 1500.f;
	data[Projectile::RifleBullet].texture = Textures::HandgunBullet;
	data[Projectile::ShotgunBullet].damage = 40;
	data[Projectile::ShotgunBullet].speed = 1500.f;
	data[Projectile::ShotgunBullet].texture = Textures::HandgunBullet;

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::HealthRefill;
	data[Pickup::HealthRefill].action = [](Character& c) {c.heal(25);  };

	data[Pickup::HandgunAmmo].texture = Textures::HandgunAmmo;
	data[Pickup::HandgunAmmo].action = [](Character& c) {c.increaseAmmo(1);  };

	data[Pickup::ShotgunAmmo].texture = Textures::ShotgunAmmo;
	data[Pickup::ShotgunAmmo].action = [](Character& c) {c.increaseAmmo(2);  };

	data[Pickup::RifleAmmo].texture = Textures::RifleAmmo;
	data[Pickup::RifleAmmo].action = [](Character& c) {c.increaseAmmo(3);  };

	data[Pickup::FireRate].texture = Textures::FireRate;
	data[Pickup::FireRate].action = std::bind(&Character::increaseFireRate, _1);

	data[Pickup::ShotgunItem].texture = Textures::ShotgunItem;
	data[Pickup::ShotgunItem].action = [](Character& c) {c.addGun(2); };

	data[Pickup::RifleItem].texture = Textures::RifleItem;
	data[Pickup::RifleItem].action = [](Character& c) {c.addGun(3); };

	return data;
}

std::vector<WeaponData> initializeWeaponData()
{
	std::vector<WeaponData> data(3);

	data[Character::HANDGUN].id = 1;
	data[Character::HANDGUN].maxAmmo = 16;
	data[Character::HANDGUN].currentAmmo = 16;
	data[Character::HANDGUN].totalAmmo = 200;
	data[Character::HANDGUN].available = true;

	data[Character::SHOTGUN].id = 2;
	data[Character::SHOTGUN].maxAmmo = 8;
	data[Character::SHOTGUN].currentAmmo = 8;
	data[Character::SHOTGUN].totalAmmo = 48;
	data[Character::SHOTGUN].available = false;

	data[Character::RIFLE].id = 3;
	data[Character::RIFLE].maxAmmo = 60;
	data[Character::RIFLE].currentAmmo = 60;
	data[Character::RIFLE].totalAmmo = 240;
	data[Character::RIFLE].available = false;

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(Particle::ParticleCount);

	data[Particle::Fire].color = sf::Color(255, 255, 50);
	data[Particle::Fire].lifetime = sf::seconds(0.6f);
	
	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].lifetime = sf::seconds(0.5f);

	data[Particle::Blood].color = sf::Color(255, 0, 0);
	data[Particle::Blood].lifetime = sf::seconds(0.3f);


	return data;
}
