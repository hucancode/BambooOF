#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Structure.h"
#include "Tool.h"
#define ALERT_TIME 5.0f
#define IMAGE_EDIT_TOOL 0
#define COLLISION_EDIT_TOOL 1
#define STRUCTURE_EXPORT_TOOL 2
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		Structure* structure;
		vector<Tool*> tools;
		int toolIndex;
		ofxPanel panelTool;
		ofxButton buttonEditImage;
		ofxButton buttonEditCollision;
		ofxButton buttonExportStructure;
		ofxLabel labelAlertMessage;
		ofxGuiGroup groupEditImage;
		ofxToggle toggleEnableMagnet;
		ofxToggle toggleDrawMagnet;
		ofxGuiGroup groupEditCollision;
		ofxButton buttonParallegram;
		ofxButton buttonTriangle;
		ofxButton buttonCircle;
		float alertTimer;
	public:
		void alertMessage(string message);
		void invokeEditImageTool();
		void invokeEditCollisionTool();
		void invokeExportStructureTool();
		void invokeEnableMagnet(bool &enable);
		void invokeDrawMagnet(bool &enable);
		void invokeParallelgram();
		void invokeTriangle();
		void invokeCircle();
};
