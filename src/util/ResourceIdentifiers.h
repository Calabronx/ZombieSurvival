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
		Zombie,
		Background,
		TitleScreen,
		HandgunBullet,
		ShotgunBullet,
		HealthRefill,
		FireSpread,
		FireRate
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
