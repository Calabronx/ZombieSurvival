#pragma once
#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <SFML/Graphics.hpp>
#include <array>

#include "../util/FileSystem.h"
#include "../util/ResourceIdentifiers.h"
#include "../ecs/SceneNode.h"
#include "../entity/Character.h"
#include "../input/command/CommandQueue.h"

namespace sf
{
	class RenderWindow;
}

class GameWorld : sf::NonCopyable
{
	public:
		explicit GameWorld(sf::RenderWindow& window, FontHolder& fonts);
		void update(sf::Time dt);
		void draw();

        CommandQueue& getCommandQueue();

		bool	hasAlivePlayer() const;
		bool	hasPlayerSurvived() const;

	private:
		void loadTextures();
		void buildScene();
		void addEnemies();
		void addEnemy(Character::Type type, float relX, float relY);
		void spawnEnemies();
		void spawnGun(int gunNum);
		void enemiesChaseIfClose();
		void destroyEntitiesOutsideView();

		void adaptPlayerPosition();
		void adaptPlayerVelocity();
		void adaptPlayerDirection();
		//void setPlayerRef(InputHandler& input);

		sf::FloatRect getViewBounds() const;
		sf::FloatRect getBattlefieldBounds() const;

		//bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

		void handleCollisions();
	private:
		enum Layer
		{
			Background,
			Land,
			LayerCount
		};

		struct SpawnPoint
		{
			SpawnPoint(Character::Type type, float x, float y)
				: type(type)
				, x(x)
				, y(y)
			{
			}
			Character::Type type;
			float x;
			float y;
		};

	private:
		sf::RenderWindow& mWindow;
		sf::View			mWorldView;
		sf::Text			mPlayerHealth;
		sf::Text			mPlayerAmmo;
		TextureHolder		mTextures;
		FontHolder&			mFonts;

		SceneNode			mSceneGraph;
		std::array<SceneNode*, LayerCount>	mSceneLayers;

		sf::FloatRect	mWorldBounds;
		sf::Vector2f	mSpawnPosition;
		float mScrollSpeed;
		int	  mHordeLevel;
		int	  mProjectiles;
		bool  mLevelsFinished;
		Character*		    mPlayerSurvivor;
        CommandQueue    mCommandQueue;
		std::vector<SpawnPoint> mEnemySpawnPoints;
		std::vector<Character*> mActiveEnemies;


};
#endif // !GAMEWORLD_H

