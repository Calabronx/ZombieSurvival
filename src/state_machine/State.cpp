#include "State.h"
#include "StateStack.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, InputHandler& inputPlayer, MusicPlayer& music, SoundPlayer& sound, HighScore& scores)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, inputPlayer(&inputPlayer)
	, sounds(&sound)
	, music(&music)
	, scores(&scores)
{
}

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)

{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}

