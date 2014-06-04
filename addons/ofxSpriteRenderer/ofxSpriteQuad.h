#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"

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
	QUAD_VISIBILITY_FAR_SCREEN
};

const float g_FarScreenThreshold = 1000;
const int g_FarScreenUpdateSequence = 20;
// 
//!!!!! NEW IDEA
// if a sprite gone too far(distance >= g_FarScreenThreshold), it will turn on m_FarFromScreen attribute
// once a sprite being far from screen, it will be ignored(pause updating) for some(g_FarScreenUpdateSequence) frame
// 
class ofxSpriteCommand;
class ofxSpriteRenderer;
class ofxSpriteQuad
{
	friend ofxSpriteCommand;
	friend ofxSpriteRenderer;
private:
	ofxSpriteMaterial* m_Material;
	unsigned int m_IndexInCommand;
	ofxSpriteCommand* m_ParentCommand;
	unsigned int m_IndexInRenderer;
	bool m_Transparent;
	QUAD_STATUS m_Status;
	QUAD_VISIBILITY m_Visibility;
private:
	bool m_ScreenPositionUpdated;
	bool m_DistanceUpdated;
	float m_DistanceToCamera;
	ofVec3f m_ScreenPosition;
	ofVec3f m_WorldPosition;
	GLfloat m_WorldQuad[2];
	unsigned short m_Quad[2];
private:
	unsigned short* m_TextureRect;
	unsigned short* m_SpriteRect;
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	void CalculateScreenPosition(const ofVec3f camera_position, bool fast_move = true);
	float CalculateDistanceToCamera(const ofVec3f camera_position);
	void SetPosition(const ofVec3f position, bool fast_move = true);
	ofxSpriteMaterial* GetMaterial()
	{
		return m_Material;
	}
	void SetMaterial(ofxSpriteMaterial* material);
	ofVec3f GetPosition()
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
	ofVec3f GetScreenPosition()
	{
		return m_ScreenPosition;
	}
	bool IsScreenPositionUpdated()
	{
		return m_ScreenPositionUpdated;
	}
	unsigned int GetWidth()
	{
		return m_Quad[0];
	}
	void SetWidth(const unsigned int width)
	{
		m_Quad[0] = width;
	}
	unsigned int GetHeight()
	{
		return m_Quad[1];
	}
	void SetHeight(const unsigned int height)
	{
		m_Quad[1] = height;
	}
	unsigned int GetTextureRectRaw(const int index)
	{
		return m_TextureRect[index];
	}
	unsigned int GetTextureRectX(const int index)
	{
		return m_TextureRect[index*4];
	}
	unsigned int GetTextureRectY(const int index)
	{
		return m_TextureRect[index*2+1];
	}
	unsigned int GetTextureRectW(const int index)
	{
		return m_TextureRect[index*4+2];
	}
	unsigned int GetTextureRectH(const int index)
	{
		return m_TextureRect[index*4+3];
	}
	unsigned int GetSpriteRectRaw(const int index)
	{
		return m_SpriteRect[index];
	}
	unsigned int GetSpriteRectX(const int index)
	{
		return m_SpriteRect[index*4];
	}
	unsigned int GetSpriteRectY(const int index)
	{
		return m_SpriteRect[index*2+1];
	}
	unsigned int GetSpriteRectW(const int index)
	{
		return m_SpriteRect[index*4+2];
	}
	unsigned int GetSpriteRectH(const int index)
	{
		return m_SpriteRect[index*4+3];
	}
	void SetMaxTexture(const int size);
	void SetTextureRect(const unsigned short order,
		const unsigned short x, const unsigned short y, 
		const unsigned short w, const unsigned short h);
	void SetSpriteRect(const unsigned short order,
		const unsigned short x, const unsigned short y, 
		const unsigned short w, const unsigned short h);
	void SubmitChanges();
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;