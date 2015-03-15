#include "ofApp.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "ImageEditTool.h"
#include "CollisionEditTool.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Structure Editor");
	ilInit();
	iluInit();
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);
	structure = new Structure();
	
	ofEnableAlphaBlending();

	tools.push_back(new ImageEditTool());
	tools.push_back(new CollisionEditTool());

	for (int i = 0; i < tools.size(); i++)
	{
		tools[i]->setParent(this);
	}
	toolIndex = 0;
	tools[toolIndex]->enter();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
	
	tools[toolIndex]->draw();
	structure->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case ' ':
		tools[toolIndex]->leave();
		toolIndex++;
		if (toolIndex == tools.size())
		{
			toolIndex = 0;
		}
		tools[toolIndex]->enter();
	case 'e':
		structure->export("hehe");
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	tools[toolIndex]->mouseDrag(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	tools[toolIndex]->mouseDown(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	tools[toolIndex]->mouseUp(x, y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
