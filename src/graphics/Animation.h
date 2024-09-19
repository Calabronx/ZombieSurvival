#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include "../util/ResourceIdentifiers.h"
#include "../util/FileSystem.h"
#include <vector>

class Animation : public sf::Drawable, public sf::Transformable
{
	public:
		Animation();
		explicit Animation(const sf::Texture& texture);

		// para texturas de tipo atlas que vengan todos los frames en una sola imagen
		void setTexture(const sf::Texture& texture);
		const sf::Texture* getTexture() const;

		// para texturas que vengan separadas por frame
		void addTextureFrame(const sf::Texture& textureFrame);
		const std::vector<sf::Texture> getTextureFrames() const;

		void setFrameSize(sf::Vector2i mFrameSize);
		sf::Vector2i getFrameSize() const;

		void setNumFrames(std::size_t numFrames);
		std::size_t getNumFrames() const;

		void setDuration(sf::Time duration);
		sf::Time getDuration() const;

		void setRepeating(bool flag);
		bool isRepeating() const;

		void restart();
		bool isFinished() const;

		sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;

		void update(sf::Time dt);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Sprite mSprite;
		sf::Vector2i mFrameSize;
		std::size_t mNumFrames;
		std::size_t mCurrentFrame;
		std::size_t mIndex;
		sf::Time mDuration;
		sf::Time mElapsedTime;
		std::vector<sf::Texture> mTextureFrames;
		bool mRepeat;
};

#endif

