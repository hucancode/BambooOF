#pragma once
#include "ofMain.h"
#define PIVOT_RADIUS 20
#define PIVOT_FONT_SIZE 16
#define PIVOT_FONT_NAME "StencilStd.otf"
class StructurePivot
{
public:
	int x, y;
	std::string name;
	bool disabled;
private:
	int ox, oy;
	bool dragged;
	static ofTrueTypeFont font;
public:
	StructurePivot();
	~StructurePivot();
	bool isDragged();
	void draw();
	void forceDrag(int mx, int my);
	void mouseDown(int mx, int my);
	void mouseUp();
	void mouseDrag(int mx, int my);
};