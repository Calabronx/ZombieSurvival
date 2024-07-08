#include "Game.h"
#include "../util/FileSystem.h"

#include<iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(640, 480), "TestGame", sf::Style::Close)
	, mWorld(mWindow)
{
	//mWindow.setMouseCursorVisible(false);
}

void Game::processInput()
{
    CommandQueue& commands = mWorld.getCommandQueue();

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
        mPlayer.handleEvent(event, commands);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

    mPlayer.handleRealTimeInput(commands);
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
			processInput();
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

