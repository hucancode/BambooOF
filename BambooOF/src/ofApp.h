#pragma once

#include "ofMain.h"
#include "ofVbo.h"
#include "ofxOrthoCamera.h"
#include "NavMeshRender.h"

#define NUM_BILLBOARDS 5000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	private:
		NavMesh* mesh;
		NavMeshRender* render;
		ofxOrthoCamera* cam;
		ofVboMesh* spriteMesh;
};