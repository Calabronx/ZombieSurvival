#include "SoundPlayer.h"

#include <SFML/Audio/Listener.hpp>

namespace
{
    // Sound coordinate system, point of view of a player in front of the screen
    // X = left,; Y = up; Z = back (out of the screen)
    const float ListenerZ = 300.f;
    const float Attenuation = 8.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}

SoundPlayer::SoundPlayer()
    : mSoundBuffers()
    , mSounds()
{
    mSoundBuffers.load(SoundEffect::Gunfire, "resources/sounds/Gunfire.wav");
    mSoundBuffers.load(SoundEffect::CollectPickup, "resources/sounds/CollectPickup.wav");
    mSoundBuffers.load(SoundEffect::Step1, "resources/sounds/step1.wav");
    mSoundBuffers.load(SoundEffect::HandgunReload, "resources/sounds/HandgunReload.wav");
    mSoundBuffers.load(SoundEffect::ShotgunReload, "resources/sounds/ShotgunReload.wav");
    mSoundBuffers.load(SoundEffect::RifleReload, "resources/sounds/AK47_Reload.wav");
    mSoundBuffers.load(SoundEffect::ZombieScream, "resources/sounds/zombieScream1.wav");

    // Listener points towards the screen (default in sfml)
    sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect)
{
    play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);

    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([](const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
     });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f position = sf::Listener::getPosition();
    return sf::Vector2f(position.x, -position.y);
}
