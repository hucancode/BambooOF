#include "ofApp.h"
#include <fstream>
//--------------------------------------------------------------
void ofApp::setup(){
	{
		ifstream fin("listfile.txt");
		fin>>rootFolder;
		while(!fin.eof())
		{
			string item;
			fin>>item;
			paths.push_back(item);
		}
		fin.close();
	}
	{
		ifstream fin("current_progress.txt");
		fin>>currentIndex;
		if(currentIndex >= paths.size())
		{
			currentIndex = 0;
		}
		fin.close();
	}
	int i = paths[currentIndex].find_last_of('/');
	folder = string(paths[currentIndex].begin(), paths[currentIndex].begin() + i+1);
	item = string(paths[currentIndex].begin() + i+1, paths[currentIndex].end());
	character.LoadOldXml(rootFolder+folder, item);
	character.Begin();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(currentIndex >= paths.size()) return;
	if(!character.Update())
	{
		{
			character.End(ofFilePath::getCurrentExeDir()+folder);
		}
		currentIndex++;
		ofstream fout("current_progress.txt");
		fout<<currentIndex;
		fout.close();
		if(currentIndex >= paths.size()) return;
		{
			int i = paths[currentIndex].find_last_of('/');
			folder = string(paths[currentIndex].begin(), paths[currentIndex].begin() + i+1);
			item = string(paths[currentIndex].begin() + i+1, paths[currentIndex].end());
			character.LoadOldXml(rootFolder+folder, item);
			character.Begin();
		}
	}
}

//--------------------------------------------------------------
#define SCALE 0.25f
void ofApp::draw(){
	ofBackgroundGradient(ofColor(100, 100, 100), ofColor(50, 50, 50));
	RectanglePacker* packer = character.GetCurrentPacker();
	if(packer)
	{
		for(int i=0;i<packer->GetNumUsedRectangle();i++)
		{
			Node* rect = packer->QueryUsedRectangle(i);
			ofGetCurrentRenderer()->setColor(rect->w/100.0*255.0, rect->h/100.0*255.0, 255);
			ofGetCurrentRenderer()->setFillMode(OF_FILLED);
			ofGetCurrentRenderer()->drawRectangle(rect->x*SCALE,rect->y*SCALE,0,rect->w*SCALE,rect->h*SCALE);
			ofGetCurrentRenderer()->setColor(0);
			ofGetCurrentRenderer()->setFillMode(OF_OUTLINE); 
			ofGetCurrentRenderer()->drawRectangle(rect->x*SCALE,rect->y*SCALE,0,rect->w*SCALE,rect->h*SCALE);
		}
		for(int i=0;i<packer->GetNumFreeRectangle();i++)
		{
			Node* rect = packer->QueryFreeRectangle(i);
			ofGetCurrentRenderer()->setColor(255, 0, 0);
			ofGetCurrentRenderer()->setFillMode(OF_OUTLINE);
			ofGetCurrentRenderer()->drawRectangle(rect->x*SCALE,rect->y*SCALE,0,rect->w*SCALE,rect->h*SCALE);
		}
		ofGetCurrentRenderer()->setColor(0);
		ofGetCurrentRenderer()->setFillMode(OF_OUTLINE);
		ofGetCurrentRenderer()->drawRectangle(0,0,0,packer->GetCurrentWidth()*SCALE,packer->GetCurrentHeight()*SCALE);
	}
	ofDrawBitmapStringHighlight("folder = "+folder+
		"\nitem = "+item+
		"\ntexture used = "+ofToString(character.GetCurrentPackerUsed())+
		"\nglobal progress = "+ofToString(currentIndex)+"/"+ofToString(paths.size())+
		"\ncurrent animation progress = "+ofToString(character.GetPercentComplete())+"%",
		10, 20);
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
