#include "ofApp.h"
#include "NavMeshRender.h"

static NavMesh* mesh;
static NavMeshRender* render;
float g_WindowAspectRatio;
//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,128,255);
    ofSetWindowTitle("Hello OF");
	//------------------
	mesh = new NavMesh;
	mesh->LoadMesh("nav_test.obj");
	mesh->BuildMesh();
	render = new NavMeshRender(mesh);
	cam.setDistance(100);
	cam.enableOrtho();
	
	g_WindowAspectRatio = 800.0/600.0;
	cam.setAspectRatio(g_WindowAspectRatio);
	cam.setScale(0.1f);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	glEnable(GL_DEPTH_TEST);
	cam.begin();
	render->Render();
	cam.end();
	glDisable(GL_DEPTH_TEST);
	//-------------------
	ofSetColor(255);
	string text = "HELLO OF\nI'M GONNA PORT BAMBOO_COCOS2D TO BAMBOO_OPENFRAMEWORKS\nP/S: CAPS LOCK FOR THE WIN!!!!";
	// * 1 character occupied 8x8 pixel
	int text_width = 432;
	int text_height = 24;
	ofDrawBitmapString(text, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	g_WindowAspectRatio = (float)w/(float)h;
	cam.setAspectRatio(g_WindowAspectRatio);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
