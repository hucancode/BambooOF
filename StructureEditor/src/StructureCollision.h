#pragma once
#include "StructurePivot.h"
#define VIEW_ANGLE PI/4.0
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
private:
	ofMesh mesh;
public:
	StructureCollision();
	~StructureCollision();
	void open();
	void close();
public:
	void draw();
	bool pick(int x, int y);
	void drag(int x, int y);
	void release();
private:
	void indentify();
	void clearChilren();
	void slice();
	void buildMesh();
};