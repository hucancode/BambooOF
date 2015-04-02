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

	toolPanel.setup("Tools");
	{
		buttonEditImage.addListener(this, &ofApp::invokeEditImageTool);
		toolPanel.add(buttonEditImage.setup("Edit Image"));
		buttonEditCollision.addListener(this, &ofApp::invokeEditCollisionTool);
		toolPanel.add(buttonEditCollision.setup("Edit Collision"));
	}
	editImagePanel.setup("Edit Image");
	{
		toggleEnableMagnet.addListener(this, &ofApp::invokeEnableMagnet);
		editImagePanel.add(toggleEnableMagnet.setup("Enable/Disable Magnet", true));
		toggleDrawMagnet.addListener(this, &ofApp::invokeDrawMagnet);
		editImagePanel.add(toggleDrawMagnet.setup("Draw/Hide Magnet", true));
	}
	editCollisionPanel.setup("Edit Collision");
	{
		buttonParallegram.addListener(this, &ofApp::invokeParallelgram);
		editCollisionPanel.add(buttonParallegram.setup("Parallegram"));
		buttonTriangle.addListener(this, &ofApp::invokeTriangle);
		editCollisionPanel.add(buttonTriangle.setup("Triangle"));
		buttonCircle.addListener(this, &ofApp::invokeCircle);
		editCollisionPanel.add(buttonCircle.setup("Circle"));
	}
	labelAlertMessage.setup("Message", "Hello, this is hu, and you are using JX STRUCTURE EDITOR!\n Have fun !!!");
	labelAlertMessage.setPosition(10, 10);
	
	alertTimer = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	float delta_time = 0.0f;
	if (ofGetFrameRate() > 1)
		delta_time = 1.0 / ofGetFrameRate();

	if (alertTimer < ALERT_TIME)
	{
		alertTimer += delta_time;
		if (alertTimer >= ALERT_TIME)
		{
			labelAlertMessage.setup("Message", "");
		}
	}
}
void ofApp::alertMessage(string message)
{
	labelAlertMessage.setName(message);
	alertTimer = 0;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
	
	tools[toolIndex]->draw();
	structure->draw();
	toolPanel.draw();
	editImagePanel.draw();
	editCollisionPanel.draw();
	labelAlertMessage.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case ' ':
		break;
		tools[toolIndex]->leave();
		toolIndex++;
		if (toolIndex == tools.size())
		{
			toolIndex = 0;
		}
		tools[toolIndex]->enter();
		break;
	case 'e':
		structure->export("hehe");
		break;
	}
}
void ofApp::invokeEditImageTool()
{
	tools[toolIndex]->leave();
	toolIndex = 0;
	tools[toolIndex]->enter();
}
void ofApp::invokeEditCollisionTool()
{
	tools[toolIndex]->leave();
	toolIndex = 1;
	tools[toolIndex]->enter();
}
void ofApp::invokeEnableMagnet(bool &enable)
{
	StructureImage::magnetEnabled = enable;
	if (!enable)
	{
		toggleDrawMagnet = false;
	}
}
void ofApp::invokeDrawMagnet(bool &enable)
{
	StructureImage::magnetVisible = enable;
}
void ofApp::invokeParallelgram()
{
	
}
void ofApp::invokeTriangle()
{

}
void ofApp::invokeCircle()
{

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
