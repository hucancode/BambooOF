#pragma once
#include "StructureCollision.h"
#include "StructureImage.h"
#include "IL/il.h"
#include "IL/ilu.h"
class Structure
{
public:
	StructureCollision* collision;
	StructureImage* image;
public:
	Structure();
	~Structure();
	bool pick(int x, int y);
	void drag(int x, int y);
	void release();
	void draw();
public:
	void import(string file_name);
	void export(string file_name);
};