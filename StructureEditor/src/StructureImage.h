#pragma once
#include "ofMain.h"
#include "IL/il.h"
#include "IL/ilu.h"
#define STRUCTURE_MIN_WIDTH 100
#define STRUCTURE_MIN_HEIGHT 100
struct StructureBasicImage
{
	ILuint id;
	ofImage image;
	ofVec2f position;
};
class StructureImage
{
public:
	static int magnetPower;
	static bool magnetEnabled;
private:
	ILuint id;
	ofImage image;
	ofVec2f position;
	vector<StructureBasicImage> children;
	vector<int> verticalMagnets;
	vector<int> horizontalMagnets;
	int width;
	int height;
private:
	bool opening;
	int draggedChild;
	int dragOffsetX;
	int dragOffsetY;
public:
	StructureImage();
	~StructureImage();
	void addChilren(string image_file);
	void open();
	bool pick(int x, int y);
	void drag(int x, int y);
	void release();
	void close();
	void draw();
	bool isOpening();
};