#pragma once
#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include<SFML/Audio/Music.hpp>
#include<SFML/System/NonCopyable.hpp>
#include "../util/ResourceIdentifiers.h"

#include<map>

class MusicPlayer : private sf::NonCopyable
{
	public:
						MusicPlayer();
		void		play(Music::ID theme);
		void		stop();

		void		setPaused(bool paused);
		void		setVolume(float volume);

	private:
		sf::Music							mMusic;
		std::map<Music::ID, std::string>	mFileNames;
		float								mVolume;
};
#endif // !MUSIC_PLAYER_H

