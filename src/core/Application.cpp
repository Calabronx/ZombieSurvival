#include "Application.h"
#include "../util/FileSystem.h"

#include<iostream>

#include "../state_machine/states/TitleState.h"
#include "../state_machine/states/MenuState.h"
#include "../state_machine/states/GameState.h"
#include "../state_machine/states/PauseState.h"
#include "../state_machine/states/GameOverState.h"
#include "../util/Utility.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(1208, 768), "Zombies Survival", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mCrossHair()
	, mPlayer()
	, mMusic()
	, mSounds()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
{
	mWindow.setMouseCursorVisible(false);
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Main, "resources/Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "resources/textures/TitleScreen.jpg");
	mTextures.load(Textures::Crosshair, "resources/textures/crosshair.png");
	mCrossHair.setTexture(mTextures.get(Textures::Crosshair));
	mCrossHair.setScale(0.12f, 0.12f);

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Title);

	mMusic.setVolume(25.f);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		mCrossHair.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));

		updateStatistics(dt);
		render();
	}
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

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);
	mWindow.draw(mCrossHair);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<GameOverState>(States::GameOver);
}