#pragma once
#ifndef  INPUT_HANDLER_H
#define  INPUT_HANDLER_H

#include <SFML/Window/Event.hpp>
#include "command/CommandQueue.h"

class InputHandler
{
public:
    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleRealTimeInput(CommandQueue& commands);
};
#endif // ! INPUT_HANDLER_H

