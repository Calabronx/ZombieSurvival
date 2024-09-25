#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include<SFML/Graphics/RenderWindow.hpp>
#include<SFML/Graphics/Text.hpp>
#include<SFML/System/Time.hpp>
#include "../input/InputHandler.h"
#include "../state_machine/StateStack.h"
#include "../util/FileSystem.h"

class Application
{
	public:
									Application();
		void			run();

	private:
		void			processInput();
		void			update(sf::Time dt);
		void			render();
		void			updateStatistics(sf::Time dt);

		void			registerStates();
	private:
		static const sf::Time		TimePerFrame;

		sf::RenderWindow		mWindow;
		TextureHolder			mTextures;
		FontHolder				mFonts;
		InputHandler			mPlayer;

		StateStack				mStateStack;

		sf::Text				mStatisticsText;
		sf::Time				mStatisticsUpdateTime;
		std::size_t				mStatisticsNumFrames;
};
#endif 
