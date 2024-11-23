#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../State.h"
#include "../../world/GameWorld.h"
#include "../../input/InputHandler.h"


class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	GameWorld		mWorld;
	InputHandler&	mPlayer;
	HighScore&		mHighScore;
};
#endif // !GAME_STATE_H

