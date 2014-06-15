#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"

#define FAR_SCREEN_DISTANCE_THRESHOLD 2.0
#define FAR_SCREEN_SPEED_THRESHOLD 0.01
#define FAR_SCREEN_UPDATE_SEQUENCE 200

/*
TODO: 
- more material operator, anti alias, mip map,  or something
- independence camera
- multi-resolution support
- particle effect support
- independence matrix & vector
*/
class ofxSpriteRenderer
{
	friend ofxSpriteCommand;
	friend ofxSpriteQuad;
public :
	static ofxSpriteRenderer* s_Instance;
	static void CreateInstance()
	{
		if(s_Instance) return;
		s_Instance = new ofxSpriteRenderer;
	}
	static ofxSpriteRenderer* GetInstance()
	{
		// NOTE: for perfomance, we not going to check instance null everytime we get it
		// programmer must create instance before they get it
		//if(!s_Instance) CreateInstance();
		return s_Instance;
	}
	static void DestroyInstance()
	{
		if(!s_Instance) return;
		delete s_Instance;
		s_Instance = 0;
	}
private:
	ofxSpriteQuads			m_SolidQuads;
	ofxSpriteQuads			m_TransparentQuads;
	ofxSpriteCommands		m_SolidCommands;
	ofxSpriteCommands		m_TransparentCommands;
	bool					m_CameraUpdated;
	vector<unsigned int>	m_UnusedSolidQuads;
	vector<unsigned int>	m_UnusedTransparentQuads;
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
	void EraseSprite(ofxSpriteQuad* sprite);
	void Update();
private:
	void BuildSolidCommands(unsigned int i, unsigned int j);
	void BuildTransparentCommands(unsigned int i, unsigned int j);
	bool CleanUnusedSolidQuads();
	bool CleanUnusedTransparentQuads();
public:
	unsigned int GetSpriteNumber()
	{
		return m_SolidQuads.size() + m_TransparentQuads.size() - m_UnusedSolidQuads.size() - m_UnusedTransparentQuads.size();
	}
	ofxOrthoCamera* GetCamera()
	{
		return m_Camera;
	}
	void MoveCamera(ofVec3f vec)
	{
		m_CameraUpdated = false;
		m_Camera->move(vec);
	}
};