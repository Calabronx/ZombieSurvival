#include "GameWorld.h"
#include "../ecs/SpriteNode.h"
#include "../util/Category.h"

#include <iostream>
#include <memory>
#include "../util/Utility.h"
#include "../entity/Pickup.h"

GameWorld::GameWorld(sf::RenderWindow& window, FontHolder& fonts)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mFonts(fonts)
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-50.f)
	, mHordeLevel(1)
	, mLevelsFinished(false)
	, mPlayerSurvivor(nullptr)
	, mEnemySpawnPoints()
	, mActiveEnemies()
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);

	//for (int i = 0; i < mTextures.)

	//std::cout << "SPAWN  X : " << mSpawnPosition.x << "  Y: " << mSpawnPosition.y << std::endl;
}

void GameWorld::update(sf::Time dt)
{
	//mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerSurvivor->setVelocity(0.f, 0.f);

	destroyEntitiesOutsideView();

	//std::cout << "ZOMBIES ALIVE: " << mActiveEnemies.size() << std::endl;
	//std::cout << "PLAYER ROTATION : " << mPlayerSurvivor->getRotation() << std::endl;
	/*std::cout << "PLAYER ROTATION : " << mPlayerSurvivor->getRotation() << std::endl;*/
	//std::cout << "PY VEC (X: " << mPlayerSurvivor->getPosition().x << ",Y: " << mPlayerSurvivor->getPosition().y << ")" << std::endl;
	//std::cout << "PY Velocity VEC (X: " << mPlayerSurvivor->getVelocity().x << ",Y: " << mPlayerSurvivor->getVelocity().y << ")" << std::endl;
	enemiesChaseIfClose();
	// Forward commands to the scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();
	adaptPlayerDirection();

	handleCollisions();

	mSceneGraph.removeWrecks();
	spawnEnemies();

	if (mActiveEnemies.size() == 0) {
		//std::cout << "spawn enemies" << std::endl;
		addEnemies();
	}

	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();
}

void GameWorld::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue& GameWorld::getCommandQueue()
{
	return mCommandQueue;
}

void GameWorld::loadTextures()
{
	//mTextures.load(Textures::Survivor, "resources/textures/handgun/idle/survivor-idle_handgun_0.png");
	mTextures.load(Textures::Survivor, "resources/textures/rifle/idle/survivor-idle_rifle_0.png");
	mTextures.load(Textures::Shoot, "resources/textures/tds_zombie/hunter_shoot.png");
	mTextures.load(Textures::Reload, "resources/textures/tds_zombie/hunter_rifle_reload.png");

	mTextures.load(Textures::ZombieIdle, "resources/textures/tds_zombie/export/skeleton-idle_0.png");
	mTextures.load(Textures::ZombieWalk, "resources/textures/tds_zombie/zombie_move_1.png");

	mTextures.load(Textures::HandgunBullet, "resources/textures/bullets/Bullet.png");
	//mTextures.load(Textures::Background, "resources/textures/Tiles/Desert.png");
	mTextures.load(Textures::Background, "resources/textures/Tiles/Asfalt1.jpg");

	mTextures.load(Textures::HealthRefill, "resources/textures/HealthRefill.png");
	mTextures.load(Textures::FireRate, "resources/textures/FireRate.png");
	mTextures.load(Textures::FireSpread, "resources/textures/FireSpread.png");

	mTextures.load(Textures::Blood, "resources/textures/blood/blood splash.png");
}

void GameWorld::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	sf::Texture& texture = mTextures.get(Textures::Background);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));


	std::unique_ptr<Pickup> healthItem(new Pickup(Pickup::HealthRefill, mTextures));
	healthItem->setPosition(0.f, 0.f);

	mSceneLayers[Land]->attachChild(std::move(healthItem));

	// agregar jugador a la escena
	std::unique_ptr<Character> player(new Character(Character::Survivor, mTextures, mFonts));
	mPlayerSurvivor = player.get();
	mPlayerSurvivor->setPosition(mSpawnPosition);
	mPlayerSurvivor->setVelocity(0.f, 0.f);
	mPlayerSurvivor->setScale(sf::Vector2f(0.300f, 0.300f));

	//mPlayerSurvivor->setRotation(
	//MOUSE VEC (X: 320,Y: 192)
	sf::FloatRect gun(mPlayerSurvivor->getGunPosition(), sf::Vector2f(4, 2));
	//mPlayerSurvivor->attachChild(gun);
	mSceneLayers[Land]->attachChild(std::move(player));

	addEnemies();
}

void GameWorld::addEnemies()
{
	switch (mHordeLevel) {
		case 1:
			addEnemy(Character::Zombie, 0.f, 500.f);
			//addEnemy(Character::Zombie, 0.f, 1000.f);
			//addEnemy(Character::Zombie, +100.f, 1100.f);
			//addEnemy(Character::Zombie, -100.f, 1100.f);
			//addEnemy(Character::Zombie, -70.f, 1400.f);
			//addEnemy(Character::Zombie, -70.f, 1600.f);
			//addEnemy(Character::Zombie, 70.f, 1400.f);
			//addEnemy(Character::Zombie, 70.f, 1600.f);
			//addEnemy(Character::Zombie, 0.f, -500.f);
			//addEnemy(Character::Zombie, 0.f, -1000.f);
			//addEnemy(Character::Zombie, +100.f, -1100.f);
			//addEnemy(Character::Zombie, -100.f, -1100.f);
			//addEnemy(Character::Zombie, -70.f, -1400.f);
			//addEnemy(Character::Zombie, -70.f, -1600.f);
			//addEnemy(Character::Zombie, 70.f, -1400.f);
			//addEnemy(Character::Zombie, 70.f, -1600.f);
			mHordeLevel++;
			break;
	/*	case 2:
			addEnemy(Character::Zombie, 0.f, 500.f);
			addEnemy(Character::Zombie, 0.f, 1000.f);
			addEnemy(Character::Zombie, +200.f, 1100.f);
			addEnemy(Character::Zombie, -200.f, 1100.f);
			addEnemy(Character::Zombie, -90.f, 1400.f);
			addEnemy(Character::Zombie, -90.f, 1600.f);
			addEnemy(Character::Zombie, 380.f, 1400.f);
			addEnemy(Character::Zombie, 390.f, 1600.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			mHordeLevel++;
			break;
		case 3:
			addEnemy(Character::Zombie, 0.f, 500.f);
			addEnemy(Character::Zombie, 0.f, 1000.f);
			addEnemy(Character::Zombie, +200.f, 1100.f);
			addEnemy(Character::Zombie, -200.f, 1100.f);
			addEnemy(Character::Zombie, -90.f, 1400.f);
			addEnemy(Character::Zombie, -90.f, 1600.f);
			addEnemy(Character::Zombie, 380.f, 1400.f);
			addEnemy(Character::Zombie, 390.f, 1600.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, 0.f, -500.f);
			addEnemy(Character::Zombie, 0.f, -1000.f);
			addEnemy(Character::Zombie, +100.f, -1100.f);
			addEnemy(Character::Zombie, -100.f, -1100.f);
			addEnemy(Character::Zombie, -70.f, -1400.f);
			addEnemy(Character::Zombie, -70.f, -1600.f);
			addEnemy(Character::Zombie, 70.f, -1400.f);
			mHordeLevel++;
			break;
		case 4:
			addEnemy(Character::Zombie, 0.f, 500.f);
			addEnemy(Character::Zombie, 0.f, 1000.f);
			addEnemy(Character::Zombie, +200.f, 1100.f);
			addEnemy(Character::Zombie, -200.f, 1100.f);
			addEnemy(Character::Zombie, -90.f, 1400.f);
			addEnemy(Character::Zombie, -90.f, 1600.f);
			addEnemy(Character::Zombie, 380.f, 1400.f);
			addEnemy(Character::Zombie, 390.f, 1600.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, 2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, -2000.f, 0.f);
			addEnemy(Character::Zombie, 0.f, -500.f);
			addEnemy(Character::Zombie, 0.f, -1000.f);
			addEnemy(Character::Zombie, +100.f, -1100.f);
			addEnemy(Character::Zombie, -100.f, -1100.f);
			addEnemy(Character::Zombie, -70.f, -1400.f);
			addEnemy(Character::Zombie, -70.f, -1600.f);
			addEnemy(Character::Zombie, 70.f, -1400.f);
			mHordeLevel++;
			break;*/
		default:
			mLevelsFinished = true;
	}

	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
}

void GameWorld::addEnemy(Character::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y + relY);
	mEnemySpawnPoints.push_back(spawn);
}

void GameWorld::spawnEnemies()
{
	while (!mEnemySpawnPoints.empty())
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Character> enemy(new Character(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setVelocity(20.f, 20.f);
		enemy->setScale(sf::Vector2f(0.300f, 0.300f));
		//enemy->setRotation(90.f); 

		mActiveEnemies.push_back(enemy.get());
		mSceneLayers[Land]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back();

	}
}

void GameWorld::enemiesChaseIfClose()
{
	// Setup command that stores all enemies in mActiveEnemies
	Command enemyCollector;
	enemyCollector.category = Category::Zombie;
	enemyCollector.action = derivedAction<Character>([this](Character& enemy, sf::Time)
		{
			if (!enemy.isDestroyed())
				mActiveEnemies.push_back(&enemy);
		});

	Command enemyChase;
	enemyChase.category = Category::Zombie;
	enemyChase.action = derivedAction<Character>([this](Character& enemyChaser, sf::Time)
		{
			if (enemyChaser.isAllied())
				return;

			float minDistance = std::numeric_limits<float>::max();
			Character* closestEnemy = nullptr;

			/*for (Character* zombie : mActiveEnemies)
			{
				float enemyDistance = distance(*mPlayerSurvivor, *zombie);

				if (enemyDistance < minDistance)
				{
					closestEnemy = zombie;
					minDistance = enemyDistance;
				}
			}*/

			//if (closestEnemy)
			enemyChaser.guideTowardsPlayer(mPlayerSurvivor->getWorldPosition());

		});

	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(enemyChase);
	mActiveEnemies.clear();
}

void GameWorld::adaptPlayerPosition()
{
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerSurvivor->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerSurvivor->setPosition(position);

}

void GameWorld::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerSurvivor->getVelocity();

	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerSurvivor->setVelocity(velocity / std::sqrt(2.f));
	//std::cout << "view: " << mWorldView.getCenter().x << " " << mWorldView.getCenter().y << std::endl;
	//std::cout << "bounds: " << mWorldBounds.getSize().x << " " << mWorldBounds.getSize().y << std::endl;

	mPlayerSurvivor->move(sf::Vector2f(0.f, 0.f));
}

void GameWorld::adaptPlayerDirection()
{
	// adaptar la direccion del jugador o el aim en base a la posicion del cursor en el juego
	sf::Time dt;
	const int ROTATION_DEGREE = 360;
	sf::Vector2i playerPosition(mPlayerSurvivor->getPosition().x, mPlayerSurvivor->getPosition().y);
	sf::Vector2i mousePosition = sf::Mouse::getPosition(mWindow);
	sf::Vector2f mouseWorldPosition = mWindow.mapPixelToCoords(mousePosition, mWorldView);

	float mouseAngle = -atan2(mouseWorldPosition.x - playerPosition.x, mouseWorldPosition.y - playerPosition.y); // angle in degrees of rotation of sprite
	float rotate = mouseAngle;

	float mouse = std::atan2(mouseWorldPosition.y, mouseWorldPosition.x);
	float degrees = toDegree(mouseAngle) + 90.f;

	mPlayerSurvivor->setMousePosition(mouseWorldPosition);
	//mPlayerSurvivor->setDirectionAngle(mouseAngle);
	mPlayerSurvivor->setRotation(degrees);

	//std::cout << "MOUSE VEC (X: " << mousePosition.x << ",Y: " << mousePosition.y << ")" << std::endl;
	//std::cout << "PLAYER ROTATION : " << mPlayerSurvivor->getRotation() << std::endl;
	//std::cout << "MOUSE ROTATION : " << mouseAngle << std::endl;
}

sf::FloatRect GameWorld::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect GameWorld::getBattlefieldBounds() const
{
	// Devuelve los bounds de la vista del juego, deberia adaptarlo para que los enemigos salgan en distintas posiciones fuera de la vista
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;
	return bounds;
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void GameWorld::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	// Deberia hacer este codigo recursivo en lugar de iterativo, revisar pag 196 del libro sflm gamedev
	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerSurvivor, Category::Zombie))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& zombie = static_cast<Character&>(*pair.second);

			const float borderDistance = 100.f;

			sf::Vector2f playerPos = player.getPosition();
			sf::Vector2f zombiePos = zombie.getPosition();

			//zombiePos.x = std::max(zombiePos.x, playerPos.x + borderDistance);
			//zombiePos.x = std::max(zombiePos.x, playerPos.x - borderDistance);
			//zombiePos.y = std::max(zombiePos.y, playerPos.y + borderDistance);
			//zombiePos.y = std::max(zombiePos.y, playerPos.y - borderDistance);
			//zombie.setPosition(zombiePos);

			//playerPos.x = std::max(playerPos.x, zombiePos.x + borderDistance);
			//playerPos.x = std::max(playerPos.x, zombiePos.x - borderDistance);
			//playerPos.y = std::max(playerPos.y, zombiePos.y + borderDistance);
			//playerPos.y = std::max(playerPos.y, zombiePos.y - borderDistance);
			//player.setPosition(playerPos);
			// X 0.160693 Y: 0.987004
			sf::Vector2f distanceVector = unitVector(zombie.getPosition() - player.getPosition());

			if (distanceVector.x <= 0.160693f && distanceVector.y >= 0.987004)
				player.damage(1);
				std::cout << "vec distance zombie X: " << distanceVector.x << "Y: " << distanceVector.y << std::endl;
		}
		else if (matchesCategories(pair, Category::PlayerSurvivor, Category::Pickup))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			pickup.apply(player);
			pickup.destroy();
		}
		else if (matchesCategories(pair, Category::Zombie, Category::Projectile))
		{
			auto& zombie = static_cast<Character&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			zombie.damage(projectile.getDamage());
			//zombie.splashBlood();
			projectile.destroy();

			//std::cout << zombie.getHitpoints() << " HP of zombie" << std::endl;

		}
		else if (matchesCategories(pair, Category::Zombie, Category::Zombie))
		{
			auto& zombie = static_cast<Character&>(*pair.first);
			auto& zombie_2 = static_cast<Character&>(*pair.second);
			
			const float borderDistance = 100.f;

			sf::Vector2f position_1 = zombie.getPosition();
			sf::Vector2f position_2 = zombie_2.getPosition();
			position_1.x = std::max(position_1.x, position_2.x + borderDistance);
			position_1.x = std::max(position_1.x, position_2.x - borderDistance);
			position_1.y = std::max(position_1.y, position_2.y + borderDistance);
			position_1.y = std::max(position_1.y, position_2.y - borderDistance);
			zombie.setPosition(position_1);


		}
	}
}

bool GameWorld::hasAlivePlayer() const
{
	return !mPlayerSurvivor->isMarkedForRemoval();
}

bool GameWorld::hasPlayerSurvived() const
{
	return mActiveEnemies.size() == 0 && mLevelsFinished;
}

void GameWorld::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingRect())) {
				e.remove();
				//std::cout << "entity destroyed" << std::endl;
			}
		});

	mCommandQueue.push(command);
}

