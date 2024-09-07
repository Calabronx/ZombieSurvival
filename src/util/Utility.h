#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <SFML/System/Vector2.hpp>
#include "../ecs/SceneNode.h"
#include "../graphics/Animation.h"

namespace sf
{
	class Sprite;
	class Text;
}

template <typename T>
std::string toString(const T& value);

float toRadian(float degree);
float toDegree(float radian);
float length(sf::Vector2f vector);
float distance(const SceneNode& lhs, const SceneNode& rhs);

int randomInt(int exclusiveMax);

sf::Vector2f unitVector(sf::Vector2f vector);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);


#include "../util/Utility.inl"

#endif // !UTILITY_H

