#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"

#define FAR_SCREEN_DISTANCE_THRESHOLD 6000.0
#define FAR_SCREEN_SPEED_THRESHOLD 30.0
#define FAR_SCREEN_UPDATE_SEQUENCE 100

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
	ofVec4f					m_WorldRect;
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
	void MoveCamera(float x, float y, float z)
	{
		m_Camera->setPosition(
			m_Camera->getPosition().x + x,
			m_Camera->getPosition().y + y,
			m_Camera->getPosition().z + z);
	}
	ofMatrix4x4 GetProjectionMatrix()
	{
		return m_Camera->GetProjectionMatrix();
	}
	ofMatrix4x4 GetModelViewMatrix()
	{
		return m_Camera->GetModelViewMatrix();
	}
	ofMatrix4x4 GetModelViewProjectionMatrix()
	{
		return m_Camera->GetModelViewProjectionMatrix();
	}
	ofMatrix4x4 GetInverseCameraMatrix()
	{
		return m_Camera->GetInverseCameraMatrix();
	}
	ofMatrix4x4 GetTransformation()
	{
		return m_TransformMatrix;
	}
	ofMatrix4x4 GetInverseModelViewMatrix()
	{
		return m_Camera->GetInverseModelViewMatrix();
	}
	ofVec4f GetRenderRect()
	{
		return m_RenderRect;
	}
	ofVec4f GetWorldRect()
	{
		return m_WorldRect;
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