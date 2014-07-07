#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"

#define FAR_SCREEN_DISTANCE_THRESHOLD 6000.0
#define FAR_SCREEN_SPEED_THRESHOLD 30.0
#define FAR_SCREEN_UPDATE_SEQUENCE 200

/*
TODO: 
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
	vector<unsigned int>	m_UnusedSolidQuads;
	vector<unsigned int>	m_UnusedTransparentQuads;
#ifdef _DEBUG
	int						m_DrawnBatches;
	int						m_DrawnVertices;
	unsigned int			m_UpdateTimeMilisecond;
	unsigned int			m_RenderTimeMilisecond;
#endif
	ofVec4f					m_RenderRect;
	ofxOrthoCamera*			m_Camera;
	ofMatrix4x4				m_TransformMatrix;
public:
	ofxSpriteRenderer();
	~ofxSpriteRenderer();
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	void EraseSprite(ofxSpriteQuad* sprite);
	void Update();
	void SetRenderSize(unsigned int width, unsigned int height);
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
	ofMatrix4x4 GetTransformation()
	{
		return m_TransformMatrix;
	}
	ofVec4f GetRenderRect()
	{
		return m_RenderRect;
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
	unsigned int GetUpdateTimeMilisecond()
	{
		return m_UpdateTimeMilisecond;
	}
	unsigned int GetRenderTimeMilisecond()
	{
		return m_RenderTimeMilisecond;
	}
#endif
};
#define ofxRENDERER ofxSpriteRenderer::GetInstance()