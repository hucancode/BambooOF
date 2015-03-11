#pragma once
#include "StructurePivot.h"
class StructureBase
{
private:
	StructurePivot* pivotTop;
	StructurePivot* pivotRight;
	StructurePivot* pivotBottom;
	StructurePivot* pivotLeft;
	vector<ofVec2f> anchors;
public:
	StructureBase();
	~StructureBase();
	void indentify();
	void clearChilren();
	void slice();
public:
	void draw();
	bool mouseDown(int x, int y);
	void mouseDrag(int x, int y);
	void mouseUp();
};