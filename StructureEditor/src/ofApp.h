#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Structure.h"
#include "Tool.h"
#define ALERT_TIME 5.0f
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
		ofxPanel panel;
		ofxButton buttonEditImage;
		ofxButton buttonEditCollision;
		ofxLabel labelAlertMessage;
		float alertTimer;
	public:
		void alertMessage(string message);
		void invokeEditImageTool();
		void invokeEditCollisionTool();
};
