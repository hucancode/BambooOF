#include "ofApp.h"
#include "TestCases.h"
//--------------------------------------------------------------
extern Test* current_test;
//--------------------------------------------------------------
float g_WindowAspectRatio;
void ofApp::setup() {
	current_test->Setup();
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetWindowTitle("Hello");
	//------------------
	mesh = new NavMesh;
	mesh->LoadMesh("nav_test.obj");
	mesh->BuildMesh();
	mesh->InitCrowd();
	render = new NavMeshRender(mesh);
	cam = ofxRENDERER->GetCamera();
	ofEnableAlphaBlending();
	
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
}

//--------------------------------------------------------------
void ofApp::update() {
	current_test->Update();
	frameIndex = (int)(ofGetFrameNum()*0.5) % images.size();
	mesh->UpdateCrowd(0.0030f);
}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofBackgroundGradient(ofColor(64), ofColor(0));
	ofBackground(ofColor(0.0f,128.0f,255.0f,255.0f));
	
	ofEnableDepthTest();
	ofEnableAlphaBlending();
	cam->begin();
	render->Render();
	
	ofDisableDepthTest();
	ofDisableAlphaBlending();
	current_test->Render();
	cam->end();
	//-------------------
#ifdef _DEBUG
	ofSetWindowTitle("FPS: "+ofToString(ofGetFrameRate(), 2)+
		" - Sprite: "+ofToString(ofxRENDERER->GetSpriteNumber())+
		" - Draw call: "+ofToString(ofxRENDERER->GetDrawCall())+
		" - Vertices: "+ofToString(ofxRENDERER->GetDrawVertices()));
#endif
}

//--------------------------------------------------------------
#define ZOOM_SPEED 0.1f
void ofApp::keyPressed(int key){
	if(key == OF_KEY_UP) 
	{
		cam->SetScale(cam->GetScale() - ZOOM_SPEED);
	}
	if(key == OF_KEY_DOWN) 
	{
		cam->SetScale(cam->GetScale() + ZOOM_SPEED);
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
			ofMatrix4x4 transform = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW)*ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
			ofVec4f position(hit_pos[0],hit_pos[1],hit_pos[2],1.0f);
			ofVec4f position_transform = transform*position;
			position_transform /= 20.0f;
			int a = 10;
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
	g_WindowAspectRatio = (float)w/(float)h;
	cam->setAspectRatio(g_WindowAspectRatio);
	ofxRENDERER->SetRenderSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
