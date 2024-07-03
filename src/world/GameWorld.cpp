#include "GameWorld.h"
#include "../ecs/SpriteNode.h"

#include <iostream>
#include <memory>

GameWorld::GameWorld(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mPlayerSurvivor(nullptr)
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mSpawnPosition);

	std::cout << "SPAWN  X : " << mSpawnPosition.x << "  Y: " << mSpawnPosition.y << std::endl;
}

void GameWorld::update(sf::Time dt)
{
	sf::Vector2f position = mPlayerSurvivor->getPosition();
	sf::Vector2f velocity = mPlayerSurvivor->getVelocity();

	if (position.x <= mWorldBounds.left + 150.f || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f)
	{
		velocity.x = -velocity.x;
		mPlayerSurvivor->setVelocity(velocity);
	}

	mSceneGraph.update(dt);
}

void GameWorld::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void GameWorld::loadTextures()
{
	mTextures.load(Textures::Survivor, "resources/textures/handgun/idle/survivor-idle_handgun_0.png");
	mTextures.load(Textures::Background, "resources/textures/Tiles/Desert.png");
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

	// agregar jugador a la escena
	std::unique_ptr<Character> player(new Character(Character::Survivor, mTextures));
	mPlayerSurvivor = player.get();
	mPlayerSurvivor->setPosition(mSpawnPosition);
	mPlayerSurvivor->setVelocity(40.f, 20.f);
	mSceneLayers[Land]->attachChild(std::move(player));
}
