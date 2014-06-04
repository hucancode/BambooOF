#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"

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
	ofxSpriteCommands	m_SolidCommands;
	ofxSpriteCommands	m_TransparentCommands;
	bool				m_RendererUpdated;
#ifdef DEBUG
	int					m_DrawnBatches;
	int					m_DrawnVertices;
#endif
	ofxOrthoCamera*		m_Camera;
public:
	ofxSpriteRenderer();
	~ofxSpriteRenderer();
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	void RemoveSprite(ofxSpriteQuad* sprite);
	void Update();
	void BuildSolidCommands(unsigned int i, unsigned int j);
	void BuildTransparentCommands(unsigned int i, unsigned int j);
	void UpdateVisibility(ofxSpriteQuad* quad);
public:
	ofxOrthoCamera* GetCamera()
	{
		return m_Camera;
	}
};