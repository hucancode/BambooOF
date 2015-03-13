#include "ofApp.h"
#include "IL/il.h"
#include "IL/ilu.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Structure Editor");
	ilInit();
	iluInit();
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);
	base = new StructureBase();
	image = new StructureImage();
	for (int i = 1; i <= 13; i++)
	{
		image->addChilren("data/2547-"+ofToString(i)+".png");
	}
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
	image->draw();
	if (!image->isOpening())
	{
		base->draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case ' ':
		if (image->isOpening())
		{
			image->close();
		}
		else
		{
			image->open();
		}
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
	base->mouseDrag(x, y);
	image->mouseDrag(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	bool swallow;
	if (!image->isOpening())
	{
		swallow = base->mouseDown(x, y);
		if (swallow)
		{
			return;
		}
	}
	swallow = image->mouseDown(x, y);
	if (swallow)
	{
		return;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	base->mouseUp();
	image->mouseUp();
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
