#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
struct ofxVertex
{
	float x,y,z;
	float u,v;
};
struct ofxSpriteCommand
{
	ofxSpriteMaterial* Material;
	ofxVertex* Vertices;
	int VerticesCount;
	int* Indices;
	int IndicesCount;
};