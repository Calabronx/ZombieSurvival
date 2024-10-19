#pragma once
#ifndef SETTINGS_STATE_H
#define SETTINGS_STATE_H

#include "../State.h"
#include "../../input/InputHandler.h"
#include "../../gui/Container.h"
#include "../../gui/Button.h"
#include "../../gui/Label.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>

class SettingsState : public State
{
public :
	SettingsState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();
	void addButtonLabel(InputHandler::Action action, float y, const std::string& text, Context context);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, InputHandler::ActionCount> mBindingButtons;
	std::array<GUI::Label::Ptr, InputHandler::ActionCount> mBindingLabels;
};
#endif // !SETTINGS_STATE_H

