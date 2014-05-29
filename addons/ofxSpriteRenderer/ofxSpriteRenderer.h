#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"
enum QUAD_UPDATE_REASON
{
	QUAD_UPDATE_REASON_MATERIAL_CHANGE,
	QUAD_UPDATE_REASON_POSITION_CHANGE,
	QUAD_UPDATE_REASON_MATERIAL_POSITION_CHANGE
};
struct ofxRequest
{
	unsigned int index;
	bool is_transparent;
	QUAD_UPDATE_REASON reason;
};
typedef vector<ofxRequest> ofxRequests;
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
	ofxSpriteQuads		m_SolidQuads;
	ofxSpriteQuads		m_TransparentQuads;
	ofxSpriteCommands	m_Commands;
	ofxRequests			m_UpdateRequest;
	bool				m_RendererUpdated;
#ifdef DEBUG
	int					m_DrawnBatches;
	int					m_DrawnVertices;
#endif
	ofxOrthoCamera*		m_Camera;
public:
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	void RemoveSprite(ofxSpriteQuad* sprite);
	void RequestUpdate(ofxRequest request);
	void SolveRequest();
	void BuildCommands();
	bool CheckVisibility(ofxSpriteQuad* quad);
public:
	ofxOrthoCamera* GetCamera()
	{
		return m_Camera;
	}
};