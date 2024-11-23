#pragma once
#ifndef  INPUT_HANDLER_H
#define  INPUT_HANDLER_H

#include <SFML/Window/Event.hpp>
#include <map>

#include "command/CommandQueue.h"

class InputHandler
{
public:
    enum Action
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Fire,
        Reload,
        ActionCount,
        MoveAim,
        EquipHandgun,
        EquipShotgun,
        EquipRifle,
        EquipKnife,
    };

    enum PlayerStatus
    {
        SurviveMode,
        Alive,
        Dead
    };

public:
                                    InputHandler();

public:
    void assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getAssignedKey(Action action) const;

    void                setPlayerStatus(PlayerStatus status);
    PlayerStatus    getPlayerStatus() const;

    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleRealTimeInput(CommandQueue& commands);

private:
    void        initializeActions();
    static bool isRealtimeAction(Action action);

private:
    std::map<sf::Keyboard::Key, Action>     mKeyBinding;
    std::map<sf::Mouse::Button, Action>         mMouseBinding;
    std::map<Action, Command>                       mActionBinding;
    PlayerStatus                                        mCurrentPlayerStatus;
};
#endif // ! INPUT_HANDLER_H

