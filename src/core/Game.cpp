#include "Game.h"
#include "../util/FileSystem.h"

#include<iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "TestGame", sf::Style::Close)
	, mWorld(mWindow)
	, mIsPaused(false)
{
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
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


void Game::update(sf::Time elapsedTime)
{
	mWorld.update(elapsedTime);
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}

