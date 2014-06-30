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
- more material operator, anti alias, or something
- independence camera
- particle effect support
- independence matrix & vector
*/
class ofxSpriteRenderer
{
	friend ofxSpriteCommand;
	friend ofxSpriteQuad;
public :
	static ofxSpriteRenderer* s_Instance;
	static ofxSpriteRenderer* GetInstance()
	{
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
#ifdef _DEBUG
	int						m_DrawnBatches;
	int						m_DrawnVertices;
#endif
	ofxOrthoCamera*			m_Camera;
	ofMatrix4x4				m_TransformMatrix;
public:
	ofxSpriteRenderer();
	~ofxSpriteRenderer();
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	void EraseSprite(ofxSpriteQuad* sprite);
	void Update();
	void SetWindowSize(unsigned int width, unsigned int height);
private:
	void BuildSolidCommands(unsigned int i, unsigned int j);
	void BuildTransparentCommands(unsigned int i, unsigned int j);
	bool CleanUnusedSolidQuads();
	bool CleanUnusedTransparentQuads();
public:
	ofxOrthoCamera* GetCamera()
	{
		return m_Camera;
	}
	void MoveCamera(ofVec3f vec)
	{
		m_CameraUpdated = false;
		m_Camera->move(vec);
	}
	ofMatrix4x4 GetTransformation()
	{
		return m_TransformMatrix;
	}
#ifdef _DEBUG
	unsigned int GetSpriteNumber()
	{
		return m_SolidQuads.size() + m_TransparentQuads.size() - m_UnusedSolidQuads.size() - m_UnusedTransparentQuads.size();
	}
	int	GetDrawCall()
	{
		return m_DrawnBatches;
	}
	int	GetDrawVertices()
	{
		return m_DrawnVertices;
	}
#endif
};
#define ofxRENDERER ofxSpriteRenderer::GetInstance()