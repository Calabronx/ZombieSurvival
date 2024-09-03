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
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x + 5000.f, 5000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-50.f)
	, mPlayerSurvivor(nullptr)
	, mEnemySpawnPoints()
	, mActiveEnemies()
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);

std::cout << "SPAWN  X : " << mSpawnPosition.x << "  Y: " << mSpawnPosition.y << std::endl;
}

void GameWorld::update(sf::Time dt)
{
	//mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerSurvivor->setVelocity(0.f, 0.f);



	destroyEntitiesOutsideView();

	std::cout << "ZOMBIES ALIVE: " << mActiveEnemies.size() << std::endl;
	//std::cout << "PLAYER ROTATION : " << mPlayerSurvivor->getRotation() << std::endl;
	/*std::cout << "PLAYER ROTATION : " << mPlayerSurvivor->getRotation() << std::endl;*/
	//std::cout << "PY VEC (X: " << mPlayerSurvivor->getPosition().x << ",Y: " << mPlayerSurvivor->getPosition().y << ")" << std::endl;
	enemiesChaseIfClose();
	// Forward commands to the scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	adaptPlayerVelocity();

	adaptPlayerDirection();

	handleCollisions();

	mSceneGraph.removeWrecks();
	spawnEnemies();


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
	//mTextures.load(Textures::Zombie, "resources/textures/zombiebasic.png");
	//mTextures.load(Textures::Zombie, "resources/textures/zombiebasic_first.png");
	mTextures.load(Textures::Zombie, "resources/textures/tds_zombie/export/skeleton-attack_0.png");
	mTextures.load(Textures::HandgunBullet, "resources/textures/bullets/Bullet.png");
	mTextures.load(Textures::Background, "resources/textures/Tiles/Desert.png");
	//mTextures.load(Textures::Background, "resources/textures/Tiles/Asfalt1.png");

	mTextures.load(Textures::HealthRefill, "resources/textures/HealthRefill.png");
	mTextures.load(Textures::FireRate, "resources/textures/FireRate.png");
	mTextures.load(Textures::FireSpread, "resources/textures/FireSpread.png");
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
	mPlayerSurvivor->setScale(sf::Vector2f(0.400f, 0.400f));

	//mPlayerSurvivor->setRotation(
	//MOUSE VEC (X: 320,Y: 192)
	sf::FloatRect gun(mPlayerSurvivor->getGunPosition(), sf::Vector2f(4, 2));
	//mPlayerSurvivor->attachChild(gun);
	mSceneLayers[Land]->attachChild(std::move(player));

	addEnemies();
}

void GameWorld::addEnemies()
{
	addEnemy(Character::Zombie, 620.f, 2000.f);
	addEnemy(Character::Zombie, 620.f, 2100.f);
	addEnemy(Character::Zombie, 620.f, 2200.f);
	addEnemy(Character::Zombie, 620.f, 2400.f);
	addEnemy(Character::Zombie, 620.f, 2500.f);
	addEnemy(Character::Zombie, 620.f, 2600.f);
	addEnemy(Character::Zombie, 620.f, 2700.f);
	addEnemy(Character::Zombie, 620.f, 2800.f);
	addEnemy(Character::Zombie, 620.f, 2900.f);
	addEnemy(Character::Zombie, 620.f, 2000.f);
	addEnemy(Character::Zombie, 620.f, 2100.f);
	addEnemy(Character::Zombie, 620.f, 2230.f);
	addEnemy(Character::Zombie, 620.f, 2700.f);
	addEnemy(Character::Zombie, 620.f, 3200.f);
	addEnemy(Character::Zombie, 620.f, 2000.f);
	addEnemy(Character::Zombie, 620.f, 2000.f);
	addEnemy(Character::Zombie, 828.f, 1205.f);
	addEnemy(Character::Zombie, 927.f, 3305.f);
	addEnemy(Character::Zombie, 326.f, 4095.f);

	addEnemy(Character::Zombie, 0.f, 1400.f);
	addEnemy(Character::Zombie, 0.f, 600.f);

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
	while (!mEnemySpawnPoints.empty() && mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Character> enemy(new Character(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setVelocity(20.f, 20.f);
		enemy->setScale(sf::Vector2f(0.400f, 0.400f));
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
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;
	const float playerVelocity = 0.10f;
	sf::Vector2f position = mPlayerSurvivor->getPosition();
	sf::Vector2f viewPosition = viewBounds.getPosition();

	if (position.x < mWorldBounds.getSize().x || position.x < 0 || position.y < mWorldBounds.getSize().y || position.y < 0) {
		//std::cout << "inside of bounds" << std::endl;
		//std::cout << "out of bounds" << std::endl;
		// el jugador es mantenido dentro de los limites del juego
		//mPlayerSurvivor->stop(true);
		position.x = std::max(position.x, mWorldView.getCenter().x);
		position.x = std::min(position.x, mWorldView.getCenter().x);
		position.y = std::max(position.y, mWorldView.getCenter().y);
		position.y = std::min(position.y, mWorldView.getCenter().y);
		sf::Vector2f delta(mPlayerSurvivor->getVelocity().x * playerVelocity, mPlayerSurvivor->getVelocity().y * playerVelocity);

		if (mPlayerSurvivor->getBoundingRect().intersects(mWorldBounds)) {
			//std::cout << "intersect bound" << std::endl;
		}


		if (viewBounds.intersects(mWorldBounds)) {
			//std::cout << "camera inside boundaries" << std::endl;
			//mPlayerSurvivor->stop(false);
		}
		else {
			//std::cout << "camera outside boundaries" << std::endl;
			//mPlayerSurvivor->stop(true);
		}
		//mWorldView.setCenter(delta);
	}
	else {
		// la camara sigue al jugador
	}

	sf::Vector2f delta(mPlayerSurvivor->getVelocity().x * playerVelocity, mPlayerSurvivor->getVelocity().y * playerVelocity);
	if (viewPosition.x < mWorldBounds.getSize().x - 400 || viewPosition.x < 0 || viewPosition.y < mWorldBounds.getSize().y - 400 || viewPosition.y < 0) {
		//std::cout << "out" << std::endl;
		mWorldView.move(delta);
	}

	position.x = std::max(position.x, mWorldBounds.left + borderDistance);
	position.x = std::min(position.x, mWorldBounds.left + mWorldBounds.width - borderDistance);
	position.y = std::max(position.y, mWorldBounds.top + borderDistance);
	position.y = std::min(position.y, mWorldBounds.top + mWorldBounds.height - borderDistance);




	//sf::Vector2f view(mWorldView.getCenter().x, mWorldView.getCenter().y);
	//mWorldView.move(view);
	//mPlayerSurvivor->setPosition(position);
	//std::cout << "pos: " << position.x << " " << position.y << std::endl;
	mPlayerSurvivor->setPosition(position);
	//mWorldView.setCenter(position);

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
	//bounds.top -= 100.f;
	//bounds.height += 100.f;
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

			player.damage(zombie.getHitpoints());
			std::cout << "Zombie collision with player " << std::endl;
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
			projectile.destroy();
			std::cout << zombie.getHitpoints() << " HP of zombie" << std::endl;

		}
	}
}

bool GameWorld::hasAlivePlayer() const
{
	return !mPlayerSurvivor->isMarkedForRemoval();
}

bool GameWorld::hasPlayerSurvived() const
{
	return mActiveEnemies.size() == 0;
}

void GameWorld::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingRect())) {
				e.destroy();
				std::cout << "entity destroyed" << std::endl;
			}
		});

	mCommandQueue.push(command);
}

