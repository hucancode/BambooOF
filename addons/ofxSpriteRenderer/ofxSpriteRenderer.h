#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteAnimation.h"

#define FAR_SCREEN_DISTANCE_THRESHOLD 6000.0
#define FAR_SCREEN_SPEED_THRESHOLD 30.0
#define FAR_SCREEN_UPDATE_SEQUENCE 100
#define IN_SCREEN_UPDATE_SEQUENCE 20

/*
TODO: 
- particle effect support
*/
class ofxSpriteRenderer
{
	friend ofxSpriteCommand;
	friend ofxSpriteQuad;
public :
	static ofxSpriteRenderer* s_Instance;
	static ofxSpriteRenderer* GetInstance();
	static void DestroyInstance();
private:
	ofxSpriteQuads			m_Quads;
	ofxSpriteCommands		m_Commands;
#ifdef _DEBUG
	int						m_DrawnBatches;
	int						m_DrawnVertices;
	unsigned int			m_UpdateTimeMilisecond;
	unsigned int			m_RenderTimeMilisecond;
#endif
	bool					m_CameraMove;
	bool					m_CameraForce;
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
	void UpdateQuads();
private:
	void BuildCommands();
public:
	void MoveCamera(float x, float y, float z);
	ofxOrthoCamera* GetCamera()
	{
		return m_Camera;
	}
	bool IsCameraMove()
	{
		return m_CameraMove;
	}
	bool IsCameraForce()
	{
		return m_CameraForce;
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
		return m_Quads.size();
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