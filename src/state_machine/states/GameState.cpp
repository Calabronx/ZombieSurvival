#include "GameState.h"
#include <iostream>

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context)
    , mWorld(*context.window, *context.fonts)
    , mPlayer(*context.inputPlayer)
{
    std::cout << "Game State started" << std::endl;
}

void GameState::draw()
{
    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealTimeInput(commands);

    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    // Game input handling
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        requestStackPush(States::Pause);

    return true;
}
