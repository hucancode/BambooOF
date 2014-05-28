#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"
enum SPRITE_UPDATE_REASON
{
	SPRITE_UPDATE_REASON_MATERIAL_CHANGE,
	SPRITE_UPDATE_REASON_POSITION_CHANGE,
	SPRITE_UPDATE_REASON_MATERIAL_POSITION_CHANGE
};
class ofxSpriteRenderer
{
public :
	static ofxSpriteRenderer* s_Instance;
	static void CreateInstance()
	{
		if(s_Instance) return;
		s_Instance = new ofxSpriteRenderer;
	}
	static ofxSpriteRenderer* GetInstance()
	{
		if(!s_Instance) CreateInstance();
		return s_Instance;
	}
	static void DestroyInstance()
	{
		if(!s_Instance) return;
		delete s_Instance;
		s_Instance = 0;
	}
private:
	vector<ofxSpriteQuad*> m_SolidQuads;
	vector<ofxSpriteQuad*> m_TransparentQuads;
	vector<ofxSpriteCommand*> m_Commands;
	vector<unsigned int> m_UpdateRequest;
	bool m_RendererUpdated;
#ifdef DEBUG
	int m_DrawnBatches;
	int m_DrawnVertices;
#endif
	ofxOrthoCamera* m_Camera;
public:
	void Render();
	void RequestUpdate(ofxSpriteQuad* sprite, SPRITE_UPDATE_REASON reason);
	void SolveRequest();
	void BuildCommands();
	bool CheckVisibility(ofxSpriteQuad* quad);
public:
	ofxOrthoCamera* GetCamera()
	{
		return m_Camera;
	}
};