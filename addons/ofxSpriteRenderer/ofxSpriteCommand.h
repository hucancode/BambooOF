#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
struct ofxVertex
{
	float x,y,z;
	float uv[64];// 32 uv quad
};
struct ofxSpriteCommand
{
	ofxSpriteMaterial* Material;
	vector<ofxVertex> Vertices;
	vector<GLushort> Indices;
	GLuint VBOID;
	GLuint IBOID;
};
#define ofxSpriteCommands vector<ofxSpriteCommand*>