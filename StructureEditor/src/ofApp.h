#pragma once

#include "ofMain.h"
#include "StructureBase.h"
#include "StructureImage.h"
#include "Tool.h"
#define ICON_WIDTH 100
#define ICON_HEIGHT 100
#define ICON_MARGIN 20
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
		StructureBase* base;
		StructureImage* image;
		vector<Tool*> tools;
		vector<ofImage> toolIcons;
		int toolIndex;
};
