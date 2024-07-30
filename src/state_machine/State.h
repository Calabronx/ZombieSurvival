#pragma once
#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "StateIdentifiers.h"
#include "../util/ResourceIdentifiers.h"

namespace sf
{
	class RenderWindow;
}

class StateStack;
class InputHandler;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;
	struct Context {
		Context(sf::RenderWindow& window, TextureHolder& textures, InputHandler& inputPlayer);
		//Context(sf::RenderWindow& window,TextureHolder& textures,FontHolder& fonts,InputHandler& inputPlayer); utilizar cuando defina una lista con las fuentes 

		sf::RenderWindow*	window;
		TextureHolder*		textures;
		InputHandler*		inputPlayer;
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

