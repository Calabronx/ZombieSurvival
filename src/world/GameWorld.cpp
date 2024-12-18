#include "GameWorld.h"
#include "../ecs/SpriteNode.h"
#include "../util/Category.h"

#include <iostream>
#include <memory>
#include "../util/Utility.h"
#include "../entity/Pickup.h"
#include "../graphics/ParticleNode.h"
#include "../graphics/EmitterNode.h"
#include "../graphics/PostEffect.h"
#include "../ecs/SoundNode.h"

GameWorld::GameWorld(sf::RenderWindow& window, FontHolder& fonts, SoundPlayer& sounds, HighScore& scores)
	: mTarget(window)
	, mWindow(window)
	, mSceneTexture()
	, mWorldView(window.getDefaultView())
	, mFonts(fonts)
	, mTextures()
	, mSounds(sounds)
	, mHighScore(scores)
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 8000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-50.f)
	, mHordeLevel(1)
	, mLevelsFinished(false)
	, mPlayerSurvivor(nullptr)
	, mEnemySpawnPoints()
	, mActiveEnemies()
	, mPlayerHealth()
	, mPlayerAmmo()
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);
	mPlayerHealth.setFont(mFonts.get(Fonts::Main));
	mPlayerHealth.setPosition(sf::Vector2f(104.f, 7916.f));
	mPlayerHealth.setCharacterSize(20);
	
	mPlayerAmmo.setFont(mFonts.get(Fonts::Main));
	mPlayerAmmo.setPosition(sf::Vector2f(104.f, 7956.f));
	mPlayerAmmo.setCharacterSize(20);
}

void GameWorld::update(sf::Time dt)
{
	//mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerSurvivor->setVelocity(0.f, 0.f);

	mPlayerHealth.setString("LIFE: " + toString(mPlayerSurvivor->getHitpoints()));
	mPlayerAmmo.setString("AMMO: " + toString(mPlayerSurvivor->getCurrentAmmunition()) + "/" + toString(mPlayerSurvivor->getCurrentTotalAmmunition()));

	if (mActiveEnemies.size() == 0)
		addEnemies();

	destroyEntitiesNotLongerUsed(dt);
	//std::cout << "ZOMBIES ALIVE: " << mActiveEnemies.size() << std::endl;

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

	updateSounds();
}

void GameWorld::draw()
{
	if (PostEffect::isSupported())
	{
		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.draw(mPlayerHealth);
		mSceneTexture.draw(mPlayerAmmo);
		mSceneTexture.draw(mCrossHair);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);
	}
	else
	{
		mWindow.setView(mWorldView);
		mWindow.draw(mSceneGraph);
		mWindow.draw(mPlayerHealth);
		mWindow.draw(mPlayerAmmo);
		mWindow.draw(mCrossHair);

	}
}

CommandQueue& GameWorld::getCommandQueue()
{
	return mCommandQueue;
}

void GameWorld::loadTextures()
{
	mTextures.load(Textures::Survivor, "resources/textures/rifle/idle/survivor-idle_rifle_0.png");

	mTextures.load(Textures::HandgunIdle, "resources/textures/tds_zombie/hunter_gun_idle.png");
	mTextures.load(Textures::HandgunMove, "resources/textures/tds_zombie/hunter_gun_move.png");
	mTextures.load(Textures::HandgunShoot, "resources/textures/tds_zombie/hunter_gun_shoot.png");

	mTextures.load(Textures::ShotgunIdle, "resources/textures/tds_zombie/hunter_shotgun_idle.png");
	mTextures.load(Textures::ShotgunMove, "resources/textures/tds_zombie/hunter_shotgun_move.png");
	mTextures.load(Textures::ShotgunShoot, "resources/textures/tds_zombie/hunter_shotgun_shoot.png");

	mTextures.load(Textures::RifleIdle, "resources/textures/tds_zombie/hunter_rifle_idle.png");
	mTextures.load(Textures::RifleMove, "resources/textures/tds_zombie/hunter_rifle_move.png");
	mTextures.load(Textures::RifleShoot, "resources/textures/tds_zombie/hunter_shoot.png");
	mTextures.load(Textures::RifleReload, "resources/textures/tds_zombie/hunter_rifle_reload.png");

	mTextures.load(Textures::ZombieIdle, "resources/textures/tds_zombie/export/skeleton-idle_0.png");
	mTextures.load(Textures::ZombieWalk, "resources/textures/tds_zombie/zombie_move.png");
	mTextures.load(Textures::ZombieAttack, "resources/textures/tds_zombie/zombie_attack.png");

	mTextures.load(Textures::HandgunBullet, "resources/textures/bullets/Bullet.png");
	mTextures.load(Textures::ShotgunBullet, "resources/textures/bullets/ShotgunBullet.png");
	//mTextures.load(Textures::Background, "resources/textures/Tiles/Desert.png");
	//mTextures.load(Textures::Background, "resources/textures/Tiles/Asfalt1.jpg");
	mTextures.load(Textures::Background, "resources/textures/Tiles/ground3.jpg");

	mTextures.load(Textures::HealthRefill, "resources/textures/HealthRefill.png");
	mTextures.load(Textures::FireRate, "resources/textures/FireRate.png");
	mTextures.load(Textures::HandgunAmmo, "resources/textures/FireSpread.png");
	mTextures.load(Textures::ShotgunAmmo, "resources/textures/ShotgunAmmo.png");
	mTextures.load(Textures::RifleAmmo, "resources/textures/RifleAmmo.png");
	mTextures.load(Textures::ShotgunItem, "resources/textures/shotgun_item.png");
	mTextures.load(Textures::RifleItem, "resources/textures/rifle_item_1.png");

	mTextures.load(Textures::Blood, "resources/textures/blood/blood splash.png");
	mTextures.load(Textures::ShootFire, "resources/textures/fire.png");

	mTextures.load(Textures::Particle, "resources/textures/particle/particle.png");
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

	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
	mSceneLayers[Land]->attachChild(std::move(smokeNode));
	
	std::unique_ptr<ParticleNode> fireNode(new ParticleNode(Particle::Fire, mTextures));
	mSceneLayers[Land]->attachChild(std::move(fireNode));
	
	std::unique_ptr<ParticleNode> bloodNode(new ParticleNode(Particle::Blood, mTextures));
	mSceneLayers[Land]->attachChild(std::move(bloodNode));

	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

	// agregar jugador a la escena
	std::unique_ptr<Character> player(new Character(Character::Survivor, mTextures, mFonts));
	mPlayerSurvivor = player.get();
	mPlayerSurvivor->setPosition(mSpawnPosition);
	mPlayerSurvivor->setVelocity(0.f, 0.f);
	mPlayerSurvivor->setScale(sf::Vector2f(0.300f, 0.300f));

	//mPlayerSurvivor->setRotation(
	//MOUSE VEC (X: 320,Y: 192)
	//mPlayerSurvivor->attachChild(gun);
	mSceneLayers[Land]->attachChild(std::move(player));

	//addEnemies();
}

void GameWorld::addEnemies()
{
	switch (mHordeLevel) {
		case 1:
			addEnemy(Character::Zombie, 0.f, 500.f);
			addEnemy(Character::Zombie, 0.f, 1000.f);
			addEnemy(Character::Zombie, +100.f, 1100.f);
			addEnemy(Character::Zombie, -100.f, 1100.f);
			addEnemy(Character::Zombie, -70.f, 1400.f);
			addEnemy(Character::Zombie, -70.f, 1600.f);
			addEnemy(Character::Zombie, 70.f, 1400.f);
			addEnemy(Character::Zombie, 70.f, 1400.f);
			addEnemy(Character::Zombie, 70.f, 1600.f);
			break;
		case 2:
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
			break;
		case 3:
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
			break;
		case 4:
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
			break;
		case 5:
			addEnemy(Character::Zombie, 0.f, 500.f);
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
			break;
		case 6:
			addEnemy(Character::Zombie, 0.f, 500.f);
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
			break;
		case 7:
			addEnemy(Character::Zombie, 0.f, 500.f);
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
			break;
		case 8:
			addEnemy(Character::Zombie, 0.f, 500.f);
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
			break;
		case 9:
			addEnemy(Character::Zombie, 0.f, 1000.f);
			addEnemy(Character::Zombie, -90.f, 1400.f);
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
			break;
		default:
			mLevelsFinished = true;
	}
	mHordeLevel++;

	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
	//std::cout << "mHorde level : " << mHordeLevel << std::endl;
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
		enemy->setScale(sf::Vector2f(0.300f, 0.300f));
		enemy->setVelocity(90.f, 90.f);

		if (mHordeLevel == 2) {
			enemy->setHitpoints(enemy->getHitpoints() + 250);
			enemy->setHitpoints(enemy->getHitpoints() + 50);
			enemy->setVelocity(sf::Vector2f(1900.f, 1900.f));

		}
		else if (mHordeLevel == 3) {
			std::unique_ptr<Pickup> shotgun(new Pickup(Pickup::ShotgunItem, mTextures));
			shotgun->setPosition(sf::Vector2f(mSpawnPosition.x + 50, mSpawnPosition.y - 450));
			shotgun->setVelocity(0.f, 50.f);
			mSceneLayers[Land]->attachChild(std::move(shotgun));
			spawnGun(2);
		}
		else if (mHordeLevel == 4) {
			enemy->setHitpoints(enemy->getHitpoints() + 290);
		}
		else if (mHordeLevel == 5) {
			enemy->setHitpoints(enemy->getHitpoints() + 350);
			spawnGun(2);
		}
		else if (mHordeLevel == 6) {
			enemy->setHitpoints(enemy->getHitpoints() + 550);
		}
		else if (mHordeLevel == 7) {
			enemy->setHitpoints(enemy->getHitpoints() + 750);
			spawnGun(3);
		}

		else if (mHordeLevel == 8) {
			enemy->setHitpoints(enemy->getHitpoints() + 950);
		}

		else if (mHordeLevel == 9) {
			enemy->setHitpoints(enemy->getHitpoints() + 1250);
			spawnGun(3);
		}
		
		//enemy->setRotation(90.f); 

		mActiveEnemies.push_back(enemy.get());
		mSceneLayers[Land]->attachChild(std::move(enemy));
		

		mEnemySpawnPoints.pop_back();

	}
}

void GameWorld::spawnGun(int gun) {
	if (mPlayerSurvivor->isGunInInventory(gun))
		return;


	if (gun == 2) {
		std::unique_ptr<Pickup> shotgun(new Pickup(Pickup::ShotgunItem, mTextures));
		shotgun->setPosition(sf::Vector2f(mSpawnPosition.x + 50, mSpawnPosition.y - 450));
		shotgun->setVelocity(0.f, 50.f);
		mSceneLayers[Land]->attachChild(std::move(shotgun));
	}
	else if (gun == 3) {
		std::unique_ptr<Pickup> rifle(new Pickup(Pickup::RifleItem, mTextures));
		rifle->setVelocity(0.f, 50.f);
		rifle->setPosition(sf::Vector2f(mSpawnPosition.x + 300, mSpawnPosition.y - 450));
		mSceneLayers[Land]->attachChild(std::move(rifle));
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

	mPlayerSurvivor->move(sf::Vector2f(0.f, 0.f));
}

void GameWorld::adaptPlayerDirection()
{
	// adaptar la direccion del jugador o el aim en base a la posicion del cursor en el juego
	sf::Time dt;
	const int ROTATION_DEGREE = 360;
	sf::Vector2i playerPosition(mPlayerSurvivor->getPosition().x, mPlayerSurvivor->getPosition().y);
	sf::Vector2i mousePosition = sf::Mouse::getPosition(mWindow);
	sf::Vector2f mouseWorldPosition = mTarget.mapPixelToCoords(mousePosition, mWorldView);

	float mouseAngle = -atan2(mouseWorldPosition.x - playerPosition.x, mouseWorldPosition.y - playerPosition.y); // angle in degrees of rotation of sprite
	float rotate = mouseAngle;

	float mouse = std::atan2(mouseWorldPosition.y, mouseWorldPosition.x);
	float degrees = toDegree(mouseAngle) + 90.f;

	mPlayerSurvivor->setMousePosition(mouseWorldPosition);
	mPlayerSurvivor->setRotation(degrees);
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

			sf::Vector2f diff;
			diff.x = playerPos.x - zombiePos.x;
			diff.y = playerPos.y - zombiePos.y;

			player.splashBlood(player.getPosition());

			float limit = 60;

			float d = length(diff);

			if (d < limit) {
				zombie.setVelocity(2.0f, 0.0f);
				player.damage(1);
				mHighScore.addScore(HighScore::DAMAGE_TAKEN, 1);
				zombie.attack();
			}
			else
				zombie.chase();
		}
		else if (matchesCategories(pair, Category::PlayerSurvivor, Category::Pickup))
		{
			auto& player = static_cast<Character&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			pickup.apply(player);
			pickup.destroy();
			
			mHighScore.addScore(HighScore::PICKUP_ITEMS, 1);
			player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
		}
		else if (matchesCategories(pair, Category::Zombie, Category::Projectile))
		{
			auto& zombie = static_cast<Character&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			zombie.damage(projectile.getDamage());
			mHighScore.addScore(HighScore::DAMAGE_MADE, projectile.getDamage());
			if (zombie.isDestroyed()) 
				mHighScore.addScore(HighScore::ZOMBIES_KILLED, 1);

			sf::Vector2f impactPos(projectile.getWorldPosition());
			zombie.splashBlood(impactPos - zombie.getWorldPosition());
			projectile.destroy();
			
		}
		else if (matchesCategories(pair, Category::Zombie, Category::Zombie))
		{
			auto& zombie = static_cast<Character&>(*pair.first);
			auto& zombie_2 = static_cast<Character&>(*pair.second);

			sf::Vector2f zombiePos = zombie_2.getPosition();
			sf::Vector2f zombiePos_2 = zombie.getPosition();
			sf::Vector2f diff;

			diff.x = zombiePos_2.x - zombiePos.x;
			diff.y = zombiePos_2.y - zombiePos.y;

			float limit = 20;
			float d = length(diff);

			//std::cout << d << std::endl;

			if (d < limit)
				zombie.setVelocity(0.0f, 10.0f);

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

void GameWorld::destroyEntitiesNotLongerUsed(sf::Time dt)
{
	Command command;
	command.category = Category::Projectile;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingRect())) {
				e.remove();
				mProjectiles++;
			}

			/*if (e.getCategory() == Category::Pickup && e.) {

			}*/
		});

	mCommandQueue.push(command);
}

void GameWorld::updateSounds()
{
	mSounds.setListenerPosition(mPlayerSurvivor->getWorldPosition());
	mSounds.removeStoppedSounds();
}

