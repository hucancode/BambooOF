#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
ofxSpriteQuad::ofxSpriteQuad()
{
	m_TextureRect = 0;
	m_Material = 0;
	m_ScreenPositionUpdated = false;
	m_FarFromScreen = false;
	m_DistanceUpdated = false;
}
void ofxSpriteQuad::SetMaterial(ofxSpriteMaterial* material)
{
	m_Material = material;
	if(m_Status != QUAD_STATUS_POSITION_CHANGE)
	{
		m_Status = QUAD_STATUS_MATERIAL_CHANGE;
	}
}
void ofxSpriteQuad::SetPosition(const ofVec3f position)
{
	m_WorldPosition = position;
	m_ScreenPositionUpdated = false;
	m_DistanceUpdated = false;
	m_Status = QUAD_STATUS_POSITION_CHANGE;
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	if(m_TextureRect) delete[] m_TextureRect;
	if(m_SpriteRect) delete[] m_SpriteRect;
}
void ofxSpriteQuad::SetMaxTexture(const int size)
{
	if(m_TextureRect || m_SpriteRect) return;
	m_TextureRect = new unsigned short[size*4];
	m_SpriteRect = new unsigned short[size*4];
}
void ofxSpriteQuad::SetTextureRect(const unsigned short order,
								   const unsigned short x, const unsigned short y, 
								   const unsigned short w, const unsigned short h)
{
	int order4 = order*4;
	m_TextureRect[order4] = x;
	m_TextureRect[++order4] = y;
	m_TextureRect[++order4] = w;
	m_TextureRect[++order4] = h;
	if(m_Status == QUAD_STATUS_NO_CHANGE)
	{
		m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
}
void ofxSpriteQuad::SetSpriteRect(const unsigned short order,
								  const unsigned short x, const unsigned short y, 
								  const unsigned short w, const unsigned short h)
{
	int order4 = order*4;
	m_SpriteRect[order4] = x;
	m_SpriteRect[++order4] = y;
	m_SpriteRect[++order4] = w;
	m_SpriteRect[++order4] = h;
	if(m_Status == QUAD_STATUS_NO_CHANGE)
	{
		m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
}
void ofxSpriteQuad::SubmitChanges()
{
	m_ParentCommand->UpdateSprite(this);
}