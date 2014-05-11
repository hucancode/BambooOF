#pragma once

#include "ofMain.h"

// Ortho camera is a custom
//	camera we've created in
//  this example
//
// We inherit from ofCamera

class ofxOrthoCamera : public ofEasyCam {
	public:
		ofMatrix4x4 ortho;
		ofxOrthoCamera();
		void begin(ofRectangle rect = ofGetWindowRect());
		ofVec3f orthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport=ofGetWindowRect());
		float scale;
};