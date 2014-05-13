#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
struct ofxSpriteQuad
{
	ofxSpriteMaterial* Material;
	ofMatrix4x4 TranslationMatrix;
	ofVec4f WorldPosition;
	ofVec2f Quad;
	ofVec2f TextureCoord;
	bool TransformUpdated;
};