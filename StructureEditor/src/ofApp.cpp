#include "ofApp.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "ImageEditTool.h"
#include "CollisionEditTool.h"
#include "StructureExportTool.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Structure Editor");
	ofEnableAlphaBlending();
	{
		ilInit();
		iluInit();
		ilEnable(IL_ORIGIN_SET);
		ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);
	}
	structure = new Structure();
	{
		tools.resize(3);
		tools[IMAGE_EDIT_TOOL] = new ImageEditTool();
		tools[COLLISION_EDIT_TOOL] = new CollisionEditTool();
		tools[STRUCTURE_EXPORT_TOOL] = new StructureExportTool();
		for (int i = 0; i < tools.size(); i++)
		{
			tools[i]->setParent(this);
		}
		toolIndex = 0;
		tools[toolIndex]->enter();
	}
	panelTool.setup("Tools");
	{
		buttonEditImage.addListener(this, &ofApp::invokeEditImageTool);
		buttonEditCollision.addListener(this, &ofApp::invokeEditCollisionTool);
		buttonExportStructure.addListener(this, &ofApp::invokeExportStructureTool);
		panelTool.add(buttonEditImage.setup("Edit Image")); 
		panelTool.add(buttonEditCollision.setup("Edit Collision"));
		panelTool.add(buttonExportStructure.setup("Export Structure"));
	}
	groupEditImage.setup("Edit Image");
	{
		toggleEnableMagnet.addListener(this, &ofApp::invokeEnableMagnet);
		toggleDrawMagnet.addListener(this, &ofApp::invokeDrawMagnet);
		groupEditImage.add(toggleEnableMagnet.setup("Enable/Disable Magnet", true));
		groupEditImage.add(toggleDrawMagnet.setup("Draw/Hide Magnet", true));
	}
	groupEditCollision.setup("Edit Collision");
	{
		buttonParallegram.addListener(this, &ofApp::invokeParallelgram);
		buttonTriangle.addListener(this, &ofApp::invokeTriangle);
		buttonCircle.addListener(this, &ofApp::invokeCircle);
		groupEditCollision.add(buttonParallegram.setup("Parallegram"));
		groupEditCollision.add(buttonTriangle.setup("Triangle"));
		groupEditCollision.add(buttonCircle.setup("Circle"));
	}
	{
		panelTool.add(&groupEditImage);
		panelTool.add(&groupEditCollision);
	}
	{
		labelAlertMessage.setup("Message", "Hello, this is hu, and you are using JX STRUCTURE EDITOR!\n Have fun !!!");
		labelAlertMessage.setPosition(10, 10);
		alertTimer = 0;
	}
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
	panelTool.draw();
	/*groupEditImage.draw();
	groupEditCollision.draw();*/
	labelAlertMessage.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
void ofApp::invokeEditImageTool()
{
	tools[toolIndex]->leave();
	toolIndex = IMAGE_EDIT_TOOL;
	tools[toolIndex]->enter();
}
void ofApp::invokeEditCollisionTool()
{
	tools[toolIndex]->leave();
	toolIndex = COLLISION_EDIT_TOOL;
	tools[toolIndex]->enter();
}
void ofApp::invokeExportStructureTool()
{
	tools[toolIndex]->leave();
	toolIndex = STRUCTURE_EXPORT_TOOL;
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
