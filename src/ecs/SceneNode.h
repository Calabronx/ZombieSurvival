#pragma once
#ifndef SCENENODE_H
#define SCENENODE_H

#include <SFML/Graphics.hpp>
#include "../input/command/Command.h"

#include <memory>
#include <vector>
#include "../input/command/CommandQueue.h"

struct Command;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	
public:
	void	attachChild(Ptr child);
	Ptr		detachChild(const SceneNode& node);

	void	update(sf::Time dt, CommandQueue& commands);


	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;


    void    onCommand(const Command& command, sf::Time dt);
    virtual unsigned int getCategory() const;

private:
	virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateChildren(sf::Time dt, CommandQueue& commands);

	virtual void		draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void				drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	SceneNode();

private:
	std::vector<Ptr>	mChildren;
	SceneNode*			mParent;
};
#endif // !SCENENODE_H

