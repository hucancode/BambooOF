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
	//scale = 1.0f;
	//cam.setDistance(100);
	g_WindowAspectRatio = 800.0/600.0;
	cam.setAspectRatio(g_WindowAspectRatio);
	cam.scale = 20;

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
	cam.begin(ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
	
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
		cam.scale -= ZOOM_SPEED;
	if(key == OF_KEY_DOWN) 
		cam.scale += ZOOM_SPEED;
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
	if(button != 0) return;
	//y = ofGetHeight() - y;
	ofVec3f ray[2];
	// Define ray in screen space
	ray[0] = ofVec3f(x, y, -1);
	ray[1] = ofVec3f(x, y, 1);

	// Transform ray into world space
	ray[0] = cam.screenToWorld(ray[0], ofGetWindowRect());
	ray[1] = cam.screenToWorld(ray[1], ofGetWindowRect());
	
	// cast it on mesh
	float* rays = new float[3];
	rays[0] = ray[0].x/cam.scale;
	rays[1] = ray[0].y/cam.scale;
	rays[2] = ray[0].z/cam.scale;
	float* raye = new float[3];
	raye[0] = ray[1].x/cam.scale;
	raye[1] = ray[1].y/cam.scale;
	raye[2] = ray[1].z/cam.scale;
	float* hit_pos = new float[3];
	float hit_ratio;
	bool hit = mesh->m_geom->raycastMesh(rays, raye, hit_ratio);
	if (hit)
	{
		hit_pos[0] = rays[0] + (raye[0] - rays[0])*hit_ratio;
		hit_pos[1] = rays[1] + (raye[1] - rays[1])*hit_ratio;
		hit_pos[2] = rays[2] + (raye[2] - rays[2])*hit_ratio;
		int ret = mesh->AddObstacle(hit_pos);
		printf("ret = %d\n",ret);
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
	cam.setAspectRatio(g_WindowAspectRatio);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}
