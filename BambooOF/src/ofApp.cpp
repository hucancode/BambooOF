#include "ofApp.h"

//--------------------------------------------------------------
float g_WindowAspectRatio;
void ofApp::setup() {
	ofBackground(0, 0, 0);
	ofSetWindowTitle("Hello");
	//------------------
	mesh = new NavMesh;
	mesh->LoadMesh("nav_test.obj");
	mesh->BuildMesh();
	mesh->InitCrowd();
	render = new NavMeshRender(mesh);

	//cam.setDistance(100);
	g_WindowAspectRatio = 800.0/600.0;
	cam.setAspectRatio(g_WindowAspectRatio);
	cam.scale = 20;

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
	planeIndex = 0;
	planes = new ofPlanePrimitive*[NUM_BILLBOARDS];
	for (int i = 0; i < NUM_BILLBOARDS; i++)
	{
		planes[i] = 0;
	}

	
}

//--------------------------------------------------------------
void ofApp::update() {
	frameIndex = (int)(ofGetFrameNum()*0.5) % images.size();
	mesh->UpdateCrowd(0.0030f);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(64), ofColor(0));
	ofSetColor(255);
	//-------------------
	cam.begin();
	
	ofEnableDepthTest();
	render->Render();
	ofSetColor(255);
	int i=0;
	while(i < planeIndex)
	{
		ofPlanePrimitive* plane = planes[i];
		ofGetCurrentRenderer()->draw(plane->getMesh(),OF_MESH_FILL,false,true,false);
		/*images[frameIndex].draw(
			plane->getPosition().x, 
			plane->getPosition().y, 
			plane->getPosition().z, 
			plane->getWidth(),
			plane->getHeight());*/
		i++;
	}
	ofDisableDepthTest();

	cam.end();
	//-------------------
	ofSetColor(255);
	ofDrawBitmapStringHighlight("FPS: "+ofToString(ofGetFrameRate(), 2)+"\nSprite: "+ofToString(planeIndex), 10, 20);
}

//--------------------------------------------------------------
#define ZOOM_SPEED 0.1f;
void ofApp::keyPressed(int key){
	if(key == OF_KEY_UP) 
		cam.scale -= ZOOM_SPEED;
	if(key == OF_KEY_DOWN) 
		cam.scale += ZOOM_SPEED;
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
void ofApp::mousePressed(int x, int y, int button){
	if(button != 0 && button != 2) return;
	ofVec3f ray[2];
	// Define ray in screen space
	ray[0] = ofVec3f(x, y, -1);
	ray[1] = ofVec3f(x, y, 1);
	// Transform ray into world space
	ray[0] = cam.orthoScreenToWorld(ray[0]);
	ray[1] = cam.orthoScreenToWorld(ray[1]);
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
			for(int i=0;i<100;i++)
			{
				if(planeIndex >= NUM_BILLBOARDS) continue;
				ofPlanePrimitive* plane = new ofPlanePrimitive();
				plane->mapTexCoords(0, 192, 192, 0);
				plane->setPosition(hit_pos[0], hit_pos[1], hit_pos[2]);
				plane->set( 2.0f, 2.0f );
				planes[planeIndex++] = plane;
			}
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
	cam.setAspectRatio(g_WindowAspectRatio);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
