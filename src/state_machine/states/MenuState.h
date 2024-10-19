#pragma once
#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "../State.h"
#include "../../gui/Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
	public:
		MenuState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);
	private:
		enum OptionNames
		{
			Play,
			Exit,
		};
	private:
		sf::Sprite					mBackgroundSprite;
		GUI::Container			mGUIContainer;
		
};
#endif // !MENU_STATE_H

