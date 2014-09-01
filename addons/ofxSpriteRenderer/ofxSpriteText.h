#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"
class ofxSpriteText
{
private:
	string m_Text;
	vector<ofxVertex> m_Vertices;
public:
	void SetText(string text);
	void BuildMesh();
};