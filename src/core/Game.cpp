#include "Game.h"
#include "../util/FileSystem.h"

#include<iostream>

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);


namespace Textures
{
	enum ID
	{
		Survivor,
		Zombie
	};
}

static FileSystem<sf::Texture, Textures::ID> textures;

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "TestVentana")
	, mPlayer()
	, mTexture()
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
{

	try {
		textures.load(Textures::Survivor, "resources/textures/handgun/idle/survivor-idle_handgun_0.png");
	}
	catch (std::runtime_error& e)
	{

		std::cout << "Error al cargar la textura survivor-idle_handgun_0.png" << e.what() << std::endl;
	}

	mPlayer.setTexture(textures.get(Textures::Survivor));
	mPlayer.setPosition(100.0f, 100.0f);
	mPlayer.setScale(sf::Vector2f(0.50f, 0.50f));

	sf::Vector2f scale = mPlayer.getScale();
	std::cout << "sprite scale: " << scale.x << " " << scale.y << std::endl;
}

void Game::run()
{
	sf::Clock clock;
	sf::Time lastTimeUpdated = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		lastTimeUpdated += elapsedTime;
		while (lastTimeUpdated > TimePerFrame)
		{
			lastTimeUpdated -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;

			case sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;

			case sf::Event::Closed:
				mWindow.close();
				break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.0f, 0.0f);

	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	mPlayer.move(movement * elapsedTime.asSeconds());
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}
