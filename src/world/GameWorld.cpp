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
    , mScrollSpeed(-50.f)
    , mPlayerSurvivor(nullptr)
{
    loadTextures();
    buildScene();

    mWorldView.setCenter(mSpawnPosition);

    std::cout << "SPAWN  X : " << mSpawnPosition.x << "  Y: " << mSpawnPosition.y << std::endl;
}

void GameWorld::update(sf::Time dt)
{
    /*mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());*/
    mPlayerSurvivor->setVelocity(0.f, 0.f);

    // Forward commands to the scene graph
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);

    adaptPlayerVelocity();

    adaptPlayerDirection();

    mSceneGraph.update(dt);
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
    mPlayerSurvivor->setVelocity(0.f, 0.f);
    mPlayerSurvivor->setScale(sf::Vector2f(0.400f, 0.400f));
    mSceneLayers[Land]->attachChild(std::move(player));
}

void GameWorld::adaptPlayerPosition()
{
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 40.f;
    sf::Vector2f position = mPlayerSurvivor->getPosition();
    //position.x = std::max(position.x, viewBounds.left + borderDistance);
    //position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    //position.y = std::max(position.y, viewBounds.top + borderDistance);
    //position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    //sf::Vector2f view(mWorldView.getCenter().x, mWorldView.getCenter().y);
    //mWorldView.move(view);
    //mPlayerSurvivor->setPosition(position);
}

void GameWorld::adaptPlayerVelocity()
{
    sf::Vector2f velocity = mPlayerSurvivor->getVelocity();

    if (velocity.x != 0.f && velocity.y != 0.f)
        mPlayerSurvivor->setVelocity(velocity / std::sqrt(2.f));

    mPlayerSurvivor->move(sf::Vector2f(0.f,0.f));
}

void GameWorld::adaptPlayerDirection()
{
    // adaptar la direccion del jugador o el aim en base a la posicion del cursor en el juego
    const int ROTATION_DEGREE = 360;
    sf::Vector2i playerPosition(mPlayerSurvivor->getPosition().x, mPlayerSurvivor->getPosition().y);
    sf::Vector2i mousePosition = sf::Mouse::getPosition(mWindow);
    sf::Vector2f mouseWorldPosition = mWindow.mapPixelToCoords(mousePosition, mWorldView);
    float mouseAngle = -atan2f(mouseWorldPosition.x - playerPosition.x, mouseWorldPosition.y - playerPosition.y) * ROTATION_DEGREE / 3.14159; // angle in degrees of rotation of sprite
    mPlayerSurvivor->setDirectionAngle(mouseAngle);
}
