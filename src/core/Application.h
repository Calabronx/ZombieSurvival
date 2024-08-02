#pragma once
#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include "../input/InputHandler.h"
#include "../state_machine/StateStack.h"
#include "../util/FileSystem.h"

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
	TextureHolder		mTextures;
	FontHolder			mFonts;
    InputHandler			mPlayer;

	StateStack				mStateStack;
};

#endif // !GAME_H
