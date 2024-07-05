#pragma once
#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <SFML/Graphics.hpp>
#include <array>

#include "../util/FileSystem.h"
#include "../util/ResourceIdentifiers.h"
#include "../ecs/SceneNode.h"
#include "../entity/Character.h"

namespace sf
{
	class RenderWindow;
}

class GameWorld : sf::NonCopyable
{
	public:
		explicit GameWorld(sf::RenderWindow& window);
		void update(sf::Time dt);
		void draw();

	private:
		void loadTextures();
		void buildScene();

	private:
		enum Layer
		{
			Background,
			Land,
			LayerCount
		};

	private:
		sf::RenderWindow& mWindow;
		sf::View			mWorldView;
		TextureHolder		mTextures;

		SceneNode			mSceneGraph;
		std::array<SceneNode*, LayerCount>	mSceneLayers;

		sf::FloatRect	mWorldBounds;
		sf::Vector2f	mSpawnPosition;
		float mScrollSpeed;
		Character*		mPlayerSurvivor;


};
#endif // !GAMEWORLD_H

