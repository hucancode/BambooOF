#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
struct ofxSpriteQuad
{
	ofxSpriteMaterial* Material;
	ofVec3f WorldPosition;
	float Quad[2];
	float* TextureCoord;
};
#define ofxSpriteQuads vector<ofxSpriteQuad*>