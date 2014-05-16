#pragma once
#include "ofMain.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
class ofxSpriteRenderer
{
public :
	static ofxSpriteRenderer* s_Instance;
	void CreateInstance()
	{
		if(s_Instance) return;
		s_Instance = new ofxSpriteRenderer;
	}
	ofxSpriteRenderer* GetInstance()
	{
		if(!s_Instance) CreateInstance();
		return s_Instance;
	}
	void DestroyInstance()
	{
		if(!s_Instance) return;
		delete s_Instance;
		s_Instance = 0;
	}
private:
	vector<ofxSpriteQuad*> m_Quads;
	vector<ofxSpriteCommand*> m_Commands;
	bool m_RendererUpdated;
#ifdef DEBUG
	int m_DrawnBatches;
	int m_DrawnVertices;
#endif
public:
	void Render();
private:
	bool SpriteCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB);
	void BuildCommands();
	bool CheckVisibility(ofxSpriteQuad quad);
};