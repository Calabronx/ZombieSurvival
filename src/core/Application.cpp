#include "Application.h"
#include "../util/FileSystem.h"

#include<iostream>

#include "../state_machine/states/TitleState.h"
#include "../state_machine/states/MenuState.h"
#include "../state_machine/states/GameState.h"
#include "../state_machine/states/PauseState.h"
#include "../state_machine/states/GameOverState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(640, 480), "TestApplication", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
{

	//mWindow.setMouseCursorVisible(false);
	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::Main, "resources/Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "resources/textures/Tiles/TitleScreen.png");

	registerStates();
	mStateStack.pushState(States::Title);
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

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

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		render();
	}
}


void Application::update(sf::Time elapsedTime)
{
	mStateStack.update(elapsedTime);
}

void Application::render()
{
	mWindow.clear();
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
	mStateStack.registerState<GameOverState>(States::GameOver);
}

