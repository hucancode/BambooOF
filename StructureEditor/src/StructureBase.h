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
	bool opening;
public:
	StructureBase();
	~StructureBase();
	void indentify();
	void clearChilren();
	void slice();
	void open();
	void close();
public:
	void draw();
	bool pick(int x, int y);
	void drag(int x, int y);
	void release();
};