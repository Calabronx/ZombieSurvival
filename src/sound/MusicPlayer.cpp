#include "MusicPlayer.h"
#include <stdexcept>

MusicPlayer::MusicPlayer()
	: mMusic()
	, mFileNames()
	, mVolume(100.f)
{
	mFileNames[Music::MenuTheme]	= "resources/music/MenuTheme.ogg";
	mFileNames[Music::MissionTheme] = "resources/music/MissionTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFileNames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}
