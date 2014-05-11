#pragma once
#include "ofMain.h"
class ofxSpriteQuad
{
private:
	ofMatrix4x4 m_TranslationMatrix;
	ofVec4f m_WorldPosition;
	ofVec2f m_Quad;
	bool m_TransformUpdated;
public:
	void Render();
	bool CheckVisibility();
};