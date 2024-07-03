#pragma once
#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H

namespace sf
{
	class Texture;
}

namespace Textures
{
	enum ID
	{
		Survivor,
		Zombie,
		Background
	};
}

// forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class FileSystem;

typedef FileSystem<sf::Texture, Textures::ID> TextureHolder;

#endif // !RESOURCEIDENTIFIERS_H
