#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>

namespace sf
{
	class Sprite;
	class Text;
}

template <typename T>
std::string toString(const T& value);

float toRadian(float degree);
float toDegree(float radian);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);


#include "../util/Utility.inl"

#endif // !UTILITY_H
