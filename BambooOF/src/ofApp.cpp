#include "ofApp.h"


float g_WindowAspectRatio;
//--------------------------------------------------------------
void ofApp::setup(){
	//ofBackground(0,128,255);
    ofSetWindowTitle("Hello");
	//------------------
	mesh = new NavMesh;
	mesh->LoadMesh("nav_test.obj");
	mesh->BuildMesh();
	render = new NavMeshRender(mesh);
	scale = 0.1f;
	cam.setDistance(100);
	cam.enableOrtho();
	g_WindowAspectRatio = 800.0/600.0;
	cam.setAspectRatio(g_WindowAspectRatio);
	cam.setScale(scale);

	ofDirectory dir;
    int nFiles = dir.listDir("plops");
    if(nFiles) {
        for(int i=0; i<dir.numFiles(); i++) {
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().loadImage(filePath);
			images.back().getTextureReference().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);
        }
    }
	frameIndex = 0;

	plane.mapTexCoords(0, 192, 192, 0);
	plane.setPosition(34.0f,10.0f,-32.0f);
	plane.set( 19.2, 19.2 );
}

//--------------------------------------------------------------
void ofApp::update(){
	frameIndex = (int)(ofGetFrameNum()*0.1) % images.size();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackgroundGradient(ofColor(64), ofColor(0));
	
	//images[frameIndex].draw(256, 36);
	//-------------------
	ofEnableDepthTest();
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	cam.begin();
	
	render->Render();
	images[frameIndex].getTextureReference().bind();
	ofSetColor(255);
	plane.draw();
	images[frameIndex].getTextureReference().unbind();
	cam.end();
	ofDisableDepthTest();
	//-------------------
	ofSetColor(255);
	string text = "HELLO !!!!!!!!!!!!!!\nRECAST FOR THE WIN!!!!";
	// * 1 character occupied 8x8 pixel
	ofDrawBitmapString(text, 10, 20);
}

#define ZOOM_SPEED 0.01f;
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//printf("key=%d\n",key);
	if(key == OF_KEY_UP) 
		scale -= ZOOM_SPEED;
	if(key == OF_KEY_DOWN) 
		scale += ZOOM_SPEED;
	cam.setScale(scale);
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
