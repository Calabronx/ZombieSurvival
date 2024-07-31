#pragma once
#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include "../world/GameWorld.h"
#include "../input/InputHandler.h"
#include "../state_machine/StateStack.h"

class Application
{

public:
	Application();
	void run();

private:
	void	processInput();
	void	update(sf::Time elapsedTime);
	void	render();

	void	registerStates();
private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow    mWindow;
	GameWorld		    mWorld;
	TextureHolder		mTextures;
	FontHolder			mFonts;
    InputHandler			mPlayer;

	StateStack				mStateStack;
};

#endif // !GAME_H
