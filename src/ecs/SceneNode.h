#pragma once
#ifndef SCENENODE_H
#define SCENENODE_H

#include <memory>
#include <vector>

class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	
public:
	void	attachChild(Ptr child);
	Ptr		detachChild(const SceneNode& node);

public:
	SceneNode();

private:
	std::vector<Ptr>	mChildren;
	SceneNode*			mParent;
};
#endif // !SCENENODE_H

