#pragma once
#ifndef HIGHSCORE_STATE_H
#define HIGHSCORE_STATE_H

#include "../State.h"
#include "../../highscore/Score.h" // deberia usar esta estructura en esta clase

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

class HighScoreState : public State
{
	public:
		HighScoreState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Text	mTitleTxt;
		sf::Text	mZombiesKilledTxt;
		sf::Text	mBulletsFiredTxt;
		sf::Text	mItemsCollectedTxt;
		sf::Text	mDamageMadeTxt;
		sf::Text	mDamageTakenTxt;
		sf::Text	mGunsCollectedTxt;
		sf::Text	mTotalDeathsTxt;
		sf::Time		mElapsedTime;

};
#endif // !HIGHSCORE_H

