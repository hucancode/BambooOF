#pragma once
class ofxCommandBase
{
protected:
	unsigned int m_RenderedVertices;
public:
	ofxCommandBase();
	~ofxCommandBase();
	virtual void Render();
	unsigned int GetRenderedVertices();
};