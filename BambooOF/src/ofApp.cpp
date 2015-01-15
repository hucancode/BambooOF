#include "ofApp.h"
#include "TestCases.h"
#include "ofxTextureCache.h"
#include "ofxShaderCache.h"
#include "ofxShaderProgramCache.h"
#include "ofxBitmapFontCache.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
//--------------------------------------------------------------
extern Test* current_test;
//--------------------------------------------------------------
void ofApp::setup() {
	ilInit();
	iluInit();
	ilutInit();
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT); 
	ilEnable(IL_SQUISH_COMPRESS);
	ilEnable(IL_NVIDIA_COMPRESS);
	new ofxSpriteRenderer;
	new ofxShaderCache;
	new ofxShaderProgramCache;
	new ofxTextureCache;
	new ofxBitmapFontCache;
	ofxRENDERER->SetRenderSize(ofGetWindowWidth(), ofGetWindowHeight());
	current_test->Setup();
	//ofSetFrameRate(60);
	ofSetWindowTitle("Bamboo OF");
	//------------------
	map = new RecastMap;
	map->LoadMesh("nav_test.obj");
	map->BuildMesh();
	map->InitCrowd();
	mapRenderer = new RecastMapRenderer(map);
	cam = ofxRENDERER->GetCamera();
	//cam->disableMouseMiddleButton();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update() {
	current_test->Update();
	map->UpdateCrowd(0.0030f);
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofBackground(ofColor(0.0f,128.0f,255.0f,255.0f));
	cam->begin();
	ofEnableDepthTest();
	mapRenderer->Render();
	ofDisableDepthTest();
	current_test->Render();
	cam->end();
#ifdef _DEBUG
	ofSetWindowTitle("FPS: "+ofToString(ofGetFrameRate(), 2)+
		" - Sprite: "+ofToString(ofxRENDERER->GetSpriteNumber())+
		" - Draw call: "+ofToString(ofxRENDERER->GetDrawCall())+
		" - Vertices: "+ofToString(ofxRENDERER->GetDrawVertices()));
#endif
}

//--------------------------------------------------------------
#define ZOOM_SPEED 15.0f
#define MOVE_SPEED 15.0f
void ofApp::keyPressed(int key){
	current_test->KeyPressed(key);
	if(key == OF_KEY_PAGE_UP) 
	{
		cam->SetScale(cam->GetScale() - ZOOM_SPEED);
	}
	if(key == OF_KEY_PAGE_DOWN) 
	{
		cam->SetScale(cam->GetScale() + ZOOM_SPEED);
	}
	if(key == OF_KEY_UP) 
	{
		ofxRENDERER->MoveCamera(0, -MOVE_SPEED);
	}
	if(key == OF_KEY_DOWN) 
	{
		ofxRENDERER->MoveCamera(0, MOVE_SPEED);
	}
	if(key == OF_KEY_LEFT) 
	{
		ofxRENDERER->MoveCamera(-MOVE_SPEED, 0);
	}
	if(key == OF_KEY_RIGHT) 
	{
		ofxRENDERER->MoveCamera(MOVE_SPEED, 0);
	}
	if(key == OF_KEY_F1)
	{
		ofxRENDERER->FocusCamera(ofVec2f(0.0f,0.0f));
	}
	if(key == OF_KEY_F2)
	{
		ofxRENDERER->Make2DCamera();
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
extern ofxBaseSprite* spriteObstacle;
void ofApp::mousePressed(int x, int y, int button){
	//return;
	if(button != 0 && button != 2) return;
	ofVec3f ray[2];
	// Define ray in screen space
	ray[0] = ofVec3f(x, y, -1);
	ray[1] = ofVec3f(x, y, 1);
	// Transform ray into world space
	ray[0] = cam->OrthoScreenToWorld(ray[0]);
	ray[1] = cam->OrthoScreenToWorld(ray[1]);
	float* rays = new float[3];
	float* raye = new float[3];
	rays[0] = ray[0].x;
	rays[1] = ray[0].y;
	rays[2] = ray[0].z;
	raye[0] = ray[1].x;
	raye[1] = ray[1].y;
	raye[2] = ray[1].z;
	float* hit_pos = new float[3];
	float hit_ratio;
	bool hit = map->GetGeometry()->raycastMesh(rays, raye, hit_ratio);
	if (hit)
	{
		hit_pos[0] = rays[0] + (raye[0] - rays[0])*hit_ratio;
		hit_pos[1] = rays[1] + (raye[1] - rays[1])*hit_ratio;
		hit_pos[2] = rays[2] + (raye[2] - rays[2])*hit_ratio;
		if(button == 0)
		{
			int ret = map->AddObstacle(hit_pos);
			map->UpdateMesh(0.0f);
			printf("ret = %d, hit pos= %f %f %f\n",ret, hit_pos[0], hit_pos[1], hit_pos[2]);
			spriteObstacle->MoveTo(ofVec3f(hit_pos[0],hit_pos[1],hit_pos[2]));
		}
		else
		{
			int ret = map->AddAgent(hit_pos);
			//mesh->UpdateMesh(0.0f);
			printf("ret = %d, hit pos= %f %f %f\n",ret, hit_pos[0], hit_pos[1], hit_pos[2]);
		}
	}
	delete[] rays;
	delete[] raye;
	delete[] hit_pos;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	ofxRENDERER->SetRenderSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
