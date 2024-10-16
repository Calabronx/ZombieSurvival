#include "InputHandler.h"
#include <SFML/System/Vector2.hpp> 
#include "../entity/Character.h"
#include "../util/Category.h"

#include <iostream>

struct SurvivorMover
{
	SurvivorMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Character& character, sf::Time) const
	{
		character.moveEntity(velocity);
	}

	sf::Vector2f velocity;
};

InputHandler::InputHandler()
	: mCurrentPlayerStatus(SurviveMode)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::A] = MoveLeft;
	mKeyBinding[sf::Keyboard::D] = MoveRight;
	mKeyBinding[sf::Keyboard::W] = MoveUp;
	mKeyBinding[sf::Keyboard::S] = MoveDown;
	mKeyBinding[sf::Keyboard::R] = Reload;
	mKeyBinding[sf::Keyboard::Num1] = EquipHandgun;
	mKeyBinding[sf::Keyboard::Num2] = EquipShotgun;
	mKeyBinding[sf::Keyboard::Num3] = EquipRifle;
	mKeyBinding[sf::Keyboard::Num4] = EquipKnife;
	mMouseBinding[sf::Mouse::Left] = Fire;

	initializeActions();

	for (auto& pair : mActionBinding)
		pair.second.category = Category::PlayerSurvivor;
}

void InputHandler::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// check if pressed key appears in key binding, trigger command if so
		auto foundKey = mKeyBinding.find(event.key.code);
		if (foundKey != mKeyBinding.end() && !isRealtimeAction(foundKey->second))
			commands.push(mActionBinding[foundKey->second]);

	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		auto foundMouse = mMouseBinding.find(event.mouseButton.button);
		if (foundMouse != mMouseBinding.end() && !isRealtimeAction(foundMouse->second))
			commands.push(mActionBinding[foundMouse->second]);
	}

	if (event.type == sf::Event::MouseMoved) {
		commands.push(mActionBinding[MoveAim]);
	}
}

void InputHandler::handleRealTimeInput(CommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}

	for (auto pair : mMouseBinding)
	{
		if (sf::Mouse::isButtonPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void InputHandler::initializeActions()
{
	const float playerSpeed = 120.f;

	mActionBinding[MoveLeft].action = derivedAction<Character>(SurvivorMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Character>(SurvivorMover(+playerSpeed, 0.f));
	mActionBinding[MoveUp].action = derivedAction<Character>(SurvivorMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Character>(SurvivorMover(0.f, +playerSpeed));
	mActionBinding[MoveAim].action = derivedAction<Character>([](Character& c, sf::Time) { c.moveAim(); });
	mActionBinding[Fire].action = derivedAction<Character>([](Character& c, sf::Time) { c.fire();  });
	mActionBinding[Reload].action = derivedAction<Character>([](Character& c, sf::Time) { c.reload(); });
	mActionBinding[EquipHandgun].action = derivedAction<Character>([](Character& c, sf::Time) {c.changeGun(1); });
	mActionBinding[EquipShotgun].action = derivedAction<Character>([](Character& c, sf::Time) {c.changeGun(2); });
	mActionBinding[EquipRifle].action = derivedAction<Character>([](Character& c, sf::Time) {c.changeGun(3); });
	mActionBinding[EquipKnife].action = derivedAction<Character>([](Character& c, sf::Time) {c.changeGun(4); });
}

bool InputHandler::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
	case MoveAim:
	case Fire:
	case Reload:
		return true;
	default:
		return false;
	}
}


void InputHandler::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto it = mKeyBinding.begin(); it != mKeyBinding.end(); )
	{
		if (it->second == action)
			mKeyBinding.erase(it++);
		else
			++it;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key InputHandler::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void InputHandler::setPlayerStatus(PlayerStatus status)
{
	mCurrentPlayerStatus = status;
}

InputHandler::PlayerStatus  InputHandler::getPlayerStatus() const
{
	return mCurrentPlayerStatus;
}
