#pragma once
#include "ofMain.h"
#include "ofxSpriteQuad.h"

class JxStructure : public ofxSpriteQuad, public ofxResource
{
private:
	vector<ofxBasicVertex> m_BasePolygon;
	vector<ofxBasicVertex> m_ModelVertices;
	vector<int> m_ModelIndices;
	float m_Height;
public:
	JxStructure();
	~JxStructure();
	virtual bool Load(string xml_file);
private:
	void BuildModel();
};