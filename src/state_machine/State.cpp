#include "State.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, InputHandler& inputPlayer)
	: window(&window)
	, textures(&textures)
	, inputPlayer(&inputPlayer)
{
}

State::State(StateStack& stack, Context context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
}

void State::requestStackPop()
{
}

void State::requestStateClear()
{
}

State::Context State::getContext() const
{
	return mContext;
}
