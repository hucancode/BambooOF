#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
struct ofxSpriteQuad
{
	ofxSpriteMaterial* Material;
	ofVec3f WorldPosition;
	ofVec2f Quad;
	ofVec2f* TextureCoord;
};
#define ofxSpriteQuads vector<ofxSpriteQuad*>