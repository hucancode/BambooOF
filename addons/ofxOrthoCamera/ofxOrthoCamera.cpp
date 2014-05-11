#include "ofxOrthoCamera.h"

// Ortho camera is a custom
//	camera we've created in
//  this example
//
// We inherit from ofCamera

ofxOrthoCamera::ofxOrthoCamera(){
	//return;
	enableOrtho();
	scale = 1;
}

void ofxOrthoCamera::begin(ofRectangle rect){
	ofEasyCam::begin(rect);
	//return;
	//--
	// Calculate aspect ratio

	float vw = ofGetViewportWidth();
	float vh = ofGetViewportHeight();

	//aspect ratio
	float ar = vw / vh;

	float scalex, scaley;

	if(ar > 1.0f){
		//wide
		scalex = scale * ar;
		scaley = scale;
	}
	else{
		//tall
		scalex = scale;
		scaley = scale / ar;
	}

	//--
	// Setup projection

	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	
    
    ortho.makeOrthoMatrix(-scalex, scalex, -scaley, scaley, -2000 * scale, 2800 * scale );
	//ortho.makeOrthoMatrix(0, rect.width, 0, rect.height, -2000 * scale, 2800 * scale );
    ofLoadMatrix( ortho );

    ofSetMatrixMode(OF_MATRIX_MODELVIEW);

	//
	//--
}
ofVec3f ofxOrthoCamera::orthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport)
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