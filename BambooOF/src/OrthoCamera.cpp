#include "OrthoCamera.h"

// Ortho camera is a custom
//	camera we've created in
//  this example
//
// We inherit from ofCamera

orthoCamera::orthoCamera(){
	//return;
	enableOrtho();
	scale = 1;
}

void orthoCamera::begin(ofRectangle rect){
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
