#pragma once

#include "ofMain.h"

// Ortho camera is a custom
//	camera we've created in
//  this example
//
// We inherit from ofCamera

class ofxOrthoCamera 
	:public ofEasyCam 
{
private:
	float m_Scale;
	float m_ScaleX;
	float m_ScaleY;
	ofRectangle m_Viewport;
public:
	ofxOrthoCamera();
	~ofxOrthoCamera();
	void begin(ofRectangle rect = ofGetWindowRect());
	ofMatrix4x4 getProjectionMatrix(ofRectangle viewport = ofGetCurrentViewport()) const;
	ofVec3f OrthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport=ofGetWindowRect());
	void SetScale(float scale);
	float GetScale();
};