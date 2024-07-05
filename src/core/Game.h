#pragma once
#ifndef GAME_H
#define GAME_H

#include<SFML/Graphics.hpp>
#include "../world/GameWorld.h"

class Game
{

public:
	Game();
	void run();

private:
	void	processEvents();
	void	update(sf::Time elapsedTime);
	void	render();

private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow mWindow;
	GameWorld		mWorld;

	sf::Texture		mTexture;
	sf::Sprite	mPlayer;

	bool mIsPaused;

};

#endif // !GAME_H
