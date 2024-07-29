#include "Application.h"
#include "../util/FileSystem.h"

#include<iostream>

#include "../state_machine/states/TitleState.h"
#include "../state_machine/states/MenuState.h"
#include "../state_machine/states/GameState.h"
#include "../state_machine/states/PauseState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(640, 480), "TestApplication", sf::Style::Close)
	, mWorld(mWindow)
	, mStateStack(State::Context(mWindow, mTextures, mPlayer))
{
	//mWindow.setMouseCursorVisible(false);
	registerStates();
	mStateStack.pushState(States::Title);
}

void Application::processInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		mPlayer.handleEvent(event, commands);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

	mPlayer.handleRealTimeInput(commands);
}

void Application::run()
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


void Application::update(sf::Time elapsedTime)
{
	mStateStack.update(elapsedTime);
	mWorld.update(elapsedTime);
}

void Application::render()
{
	mWindow.clear();
	mWorld.draw();
	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.display();
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
}

