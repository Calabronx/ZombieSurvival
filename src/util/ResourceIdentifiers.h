#pragma once
#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H

namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		Survivor,
		HandgunIdle,
		HandgunMove,
		HandgunShoot,
		HandgunReload,

		RifleIdle,
		RifleMove,
		RifleShoot,
		RifleReload,

		ShotgunIdle,
		ShotgunMove,
		ShotgunShoot,
		ShotgunReload,

		ZombieIdle,
		ZombieWalk,
		ZombieAttack,

		Background,
		TitleScreen,
		HandgunBullet,
		ShotgunBullet,
		HealthRefill,
		HandgunAmmo,
		ShotgunAmmo,
		RifleAmmo,
		FireRate,
		ShotgunItem,
		RifleItem,
		Particle,
		Blood,
		ShootFire,
		Crosshair,

		ButtonNormal,
		ButtonSelected,
		ButtonPressed
	};
}

namespace Shaders
{
	enum ID
	{
		BrithnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Label
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		MissionTheme,
		AmbientWind
	};
}

namespace SoundEffect
{
	enum ID
	{
		Gunfire,
		HandgunReload,
		ShotgunReload,
		RifleReload,
	/*	HandgunFire,
		ShotgunFire,
		RifleFire,*/
		CollectPickup,
		ZombieScream,
		Damage,
		Step1,
		Button
	};

};

// forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class FileSystem;

typedef FileSystem<sf::Texture, Textures::ID>				TextureHolder;
typedef FileSystem<sf::Font, Fonts::ID>							FontHolder;
typedef FileSystem<sf::Shader, Shaders::ID>					ShaderHolder;
typedef FileSystem<sf::SoundBuffer, SoundEffect::ID>	SoundBufferHolder;

#endif // !RESOURCEIDENTIFIERS_H
