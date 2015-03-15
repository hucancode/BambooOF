#pragma once
#include "StructurePivot.h"
class StructureCollision
{
	friend class Structure;
private:
	StructurePivot* pivotTop;
	StructurePivot* pivotRight;
	StructurePivot* pivotBottom;
	StructurePivot* pivotLeft;
	bool opening;
	vector<ofVec2f> anchors;
public:
	StructureCollision();
	~StructureCollision();
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