#pragma once
#include "ofMain.h"
#include "ofxPolyMaterial.h"
#include "ofxMonoMaterial.h"
enum QUAD_STATUS
{
	QUAD_STATUS_MATERIAL_CHANGE,
	QUAD_STATUS_POSITION_CHANGE,
	QUAD_STATUS_SAFE_CHANGE,
	QUAD_STATUS_NO_CHANGE
};
enum QUAD_VISIBILITY
{
	QUAD_VISIBILITY_IN_SCREEN,
	QUAD_VISIBILITY_OFF_SCREEN,
	QUAD_VISIBILITY_FAR_SCREEN,
	QUAD_VISIBILITY_UNKNOWN,
};

class ofxSpriteCommand;
class ofxSpriteRenderer;
class ofxSpriteQuad
{
	friend ofxSpriteCommand;
	friend ofxSpriteRenderer;
protected:
	ofxSpriteMaterial*	m_Material;
	unsigned int		m_IndexInCommand;
	ofxSpriteCommand*	m_ParentCommand;
public:
	unsigned int		m_IndexInRenderer;
protected:
	bool				m_Transparent;
	QUAD_STATUS			m_Status;
	QUAD_VISIBILITY		m_Visibility;
	ofVec3f				m_WorldPosition;
	ofVec2f				m_WorldQuad;
	bool				m_Visible;
	float				m_DistanceToCamera;
private:
	bool				m_DistanceUpdated;
protected:
	ofVec4f*			m_TextureRect;
	ofVec4f*			m_SpriteRect;
protected:
	ofVec3f				m_glPosition[4];
	ofVec4f*			m_glUV;
	ofVec4f*			m_glCUV;
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	virtual void Update(const float delta_time){}
	void MoveTo(const float x, const float y, const float z);
	void MoveTo(const ofVec3f position);
	void MoveBy(const float x, const float y, const float z);
	void MoveBy(const ofVec3f accelerator);
	void UpdateVisibility();
public:
	ofxSpriteMaterial* GetMaterial()
	{
		return m_Material;
	}
	void SetMaterial(ofxSpriteMaterial* material);
	ofVec3f GetWorldPosition()
	{
		return m_WorldPosition;
	}
	bool IsTransparent()
	{
		return m_Transparent;
	}
	void SetTransparent(bool value)
	{
		m_Transparent = value;
	}
	QUAD_VISIBILITY GetVisibility()
	{
		return m_Visibility;
	}
	float GetLogicWidth()
	{
		return m_WorldQuad.x;
	}
	void SetLogicWidth(const float width)
	{
		m_WorldQuad.x = width;
		if(m_Status == QUAD_STATUS_NO_CHANGE)
			m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
	float GetLogicHeight()
	{
		return m_WorldQuad.y;
	}
	void SetLogicHeight(const float height)
	{
		m_WorldQuad.y = height;
		if(m_Status == QUAD_STATUS_NO_CHANGE)
			m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
	void SetLogicSize(const float width, const float height)
	{
		SetLogicSize(ofVec2f(width,height));
	}
	void SetLogicSize(ofVec2f dimension)
	{
		m_WorldQuad = dimension;
		if(m_Status == QUAD_STATUS_NO_CHANGE)
			m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
public:
	ofVec4f GetTextureRect(const int index)
	{
		return m_TextureRect[index];
	}
	ofVec4f GetSpriteRect(const int index)
	{
		return m_SpriteRect[index];
	}
	void SetMaxTexture(const int size);
	void SetTextureRect(const int index, const float x, const float y, const float w, const float h);
	void SetSpriteRect(const int index, const float x, const float y, const float w, const float h);
	void SetTextureRect(const int index, const ofVec4f rect);
	void SetSpriteRect(const int index, const ofVec4f rect);
	void SubmitChanges();
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;