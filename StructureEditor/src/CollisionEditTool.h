#pragma once
#include "Tool.h"
class CollisionEditTool : public Tool
{
private:
public:
	virtual void enter();
	virtual void leave();
	virtual void draw();
	virtual void mouseDown(int x, int y);
	virtual void mouseDrag(int x, int y);
	virtual void mouseMove(int x, int y);
	virtual void mouseUp(int x, int y);
};