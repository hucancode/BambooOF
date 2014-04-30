#pragma once

#include "ofMain.h"

// Ortho camera is a custom
//	camera we've created in
//  this example
//
// We inherit from ofCamera

class orthoCamera : public ofEasyCam {
	public:
		ofMatrix4x4 ortho;
		orthoCamera();
		void begin(ofRectangle rect = ofGetWindowRect());
		ofVec3f orthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport=ofGetWindowRect())
		{
			//convert from screen to camera
			ofVec3f CameraXYZ;
			CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
			CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
			CameraXYZ.z = ScreenXYZ.z;

			//get inverse camera matrix
			ofMatrix4x4 inverseCamera;
			inverseCamera.makeInvertOf(getModelViewMatrix() * ortho);

			//convert camera to world
			return CameraXYZ * inverseCamera;
		}
		float scale;
};