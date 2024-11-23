#pragma once
#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "StateIdentifiers.h"
#include "../util/ResourceIdentifiers.h"
#include "../sound/MusicPlayer.h"
#include "../sound/SoundPlayer.h"
#include "../highscore/HighScore.h"

namespace sf
{
	class RenderWindow;
}

class StateStack;
class InputHandler;
class MusicPlayer;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;
	struct Context {
		Context(sf::RenderWindow& window,TextureHolder& textures,FontHolder& fonts,InputHandler& inputPlayer, MusicPlayer& music, SoundPlayer& sound, HighScore& scores);

		sf::RenderWindow*	window;
		TextureHolder*		textures;
		FontHolder*			fonts;
		InputHandler*		inputPlayer;
		MusicPlayer*		music;
		SoundPlayer*		sounds;
		HighScore*			scores;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStateClear();

	Context getContext() const;

private:
	StateStack* mStack;
	Context		mContext;
};
#endif // !STATE_H

