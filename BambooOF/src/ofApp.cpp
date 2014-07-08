#include "ofApp.h"
#include "TestCases.h"
//--------------------------------------------------------------
extern Test* current_test;
//--------------------------------------------------------------
void ofApp::setup() {
	new ofxSpriteRenderer;
	ofxRENDERER->SetRenderSize(ofGetWindowWidth(), ofGetWindowHeight());
	current_test->Setup();
	//ofSetFrameRate(60);
	ofSetWindowTitle("Bamboo OF");
	//------------------
	mesh = new NavMesh;
	mesh->LoadMesh("nav_test.obj");
	mesh->BuildMesh();
	mesh->InitCrowd();
	render = new NavMeshRender(mesh);
	cam = ofxRENDERER->GetCamera();
	cam->disableMouseMiddleButton();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update() {
	current_test->Update();
	mesh->UpdateCrowd(0.0030f);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(ofColor(0.0f,128.0f,255.0f,255.0f));
	
	cam->begin();
	ofEnableDepthTest();
	render->Render();
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
		ofxRENDERER->MoveCamera(0, 0, -MOVE_SPEED);
	}
	if(key == OF_KEY_DOWN) 
	{
		ofxRENDERER->MoveCamera(0, 0, MOVE_SPEED);
	}
	if(key == OF_KEY_LEFT) 
	{
		ofxRENDERER->MoveCamera(-MOVE_SPEED, 0, 0);
	}
	if(key == OF_KEY_RIGHT) 
	{
		ofxRENDERER->MoveCamera(MOVE_SPEED, 0, 0);
	}
	if(key == OF_KEY_F4) 
		render->SwitchDrawMesh();
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
extern ofxSpriteQuad* spriteObstacle;
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
	bool hit = mesh->m_geom->raycastMesh(rays, raye, hit_ratio);
	if (hit)
	{
		hit_pos[0] = rays[0] + (raye[0] - rays[0])*hit_ratio;
		hit_pos[1] = rays[1] + (raye[1] - rays[1])*hit_ratio;
		hit_pos[2] = rays[2] + (raye[2] - rays[2])*hit_ratio;
		if(button == 0)
		{
			int ret = mesh->AddObstacle(hit_pos);
			mesh->UpdateMesh(0.0f);
			printf("ret = %d, hit pos= %f %f %f\n",ret, hit_pos[0], hit_pos[1], hit_pos[2]);
			spriteObstacle->MoveTo(ofVec3f(hit_pos[0],hit_pos[1],hit_pos[2]));
		}
		else
		{
			int ret = mesh->AddAgent(hit_pos);
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
