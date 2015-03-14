#pragma once
#include "ofMain.h"
class ofApp;

class Tool
{
protected:
	ofApp* parent;
public:
	Tool();
	~Tool();
	void setParent(ofApp* app);
	virtual void enter();
	virtual void leave();
	virtual void draw();
	virtual void mouseDown(int x, int y);
	virtual void mouseDrag(int x, int y);
	virtual void mouseMove(int x, int y);
	virtual void mouseUp(int x, int y);
};