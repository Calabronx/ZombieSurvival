#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

struct Particle
{
	// fuego,smoke,blood,bullet shell

	enum Type
	{
		Fire,
		Smoke,
		Blood,
		BulletShell,
		ParticleCount
	};

	sf::Vector2f position;
	sf::Color color;
	sf::Time lifeTime;

};

#endif
