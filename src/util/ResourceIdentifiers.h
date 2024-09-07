#pragma once
#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H

namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		Survivor,

		ZombieIdle1,
		ZombieWalk1,
		ZombieWalk2,
		ZombieWalk3,
		ZombieWalk4,
		ZombieWalk5,
		ZombieWalk6,
		ZombieWalk7,
		ZombieWalk8,
		ZombieWalk9,
		ZombieWalk10,
		ZombieWalk11,
		ZombieWalk12,
		ZombieWalk13,
		ZombieWalk14,
		ZombieWalk15,
		ZombieWalk16,
		ZombieWalk17,
		Background,
		TitleScreen,
		HandgunBullet,
		ShotgunBullet,
		HealthRefill,
		FireSpread,
		FireRate,
		Particle,
		Blood
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

// forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class FileSystem;

typedef FileSystem<sf::Texture, Textures::ID> TextureHolder;
typedef FileSystem<sf::Font, Fonts::ID>			FontHolder;

#endif // !RESOURCEIDENTIFIERS_H
