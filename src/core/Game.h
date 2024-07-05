#pragma once
#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include "../world/GameWorld.h"
#include "../input/InputHandler.h"

class Game
{

public:
	Game();
	void run();

private:
	void	processInput();
	void	update(sf::Time elapsedTime);
	void	render();

private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow    mWindow;
	GameWorld		    mWorld;
    InputHandler        mPlayer;
};

#endif // !GAME_H
