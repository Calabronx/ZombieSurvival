#pragma once
#include<SFML/Graphics.hpp>

class Game
{
public:
	Game();
	void run();
private:
	void	processEvents();
	void	update(sf::Time elapsedTime);
	void	render();
	void	handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
private:
	static const float			PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow mWindow;
	sf::Texture		mTexture;
	sf::Sprite	mPlayer;

	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;

};

