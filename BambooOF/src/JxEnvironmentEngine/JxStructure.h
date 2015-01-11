#pragma once
#include "ofMain.h"
#include "ofxSpriteQuad.h"

class JxStructure : public ofxSpriteQuad, public ofxResource
{
private:
	vector<ofVec2f> m_BasePolygon;
	vector<ofVec3f> m_Model;
	float m_Height;
public:
	JxStructure();
	~JxStructure();
	virtual bool Load(string xml_file);
private:
	void BuildModel();
};