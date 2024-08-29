#pragma once
#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../State.h"

class GameOverState : public State
{
	public:
									GameOverState(StateStack& stack, Context context);

		virtual void	draw();
		virtual bool	update(sf::Time dt);
		virtual bool	handleEvent(const sf::Event& event);

	private:
		sf::Text		mGameOverText;
		sf::Time		mElapsedTime;

};
#endif // !GAME_OVER_STATE_H

