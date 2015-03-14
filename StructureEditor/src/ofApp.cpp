#include "ofApp.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "ImageEditTool.h"
#include "PivotEditTool.h"
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

	tools.push_back(new ImageEditTool());
	toolIcons.push_back(ofImage("image_icon.png"));
	tools.push_back(new PivotEditTool());
	toolIcons.push_back(ofImage("pivot_icon.png"));

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
	image->draw();
	base->draw();
	ofFill();
	ofRect(toolIndex * (ICON_WIDTH + ICON_MARGIN * 2), 0, ICON_WIDTH + ICON_MARGIN * 2, ICON_HEIGHT + ICON_MARGIN * 2);
	for (int i = 0; i<toolIcons.size(); i++)
	{
		toolIcons[i].draw(i * (ICON_WIDTH + ICON_MARGIN * 2) + ICON_MARGIN, ICON_MARGIN);
	}
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
	/*bool swallow;
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
	}*/
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
