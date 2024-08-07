#pragma once
#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "SceneNode.h"
#include "../util/ResourceIdentifiers.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>

class TextNode : public SceneNode
{
public:
	explicit	TextNode(const FontHolder& fonts, const std::string& text);
	void		setString(const std::string& text);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text mText;
};
#endif

