#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"

ofxSpriteQuad::ofxSpriteQuad()
{
	ofxSpriteBase::ofxSpriteBase();
	m_VerticesSize = 4;
	m_Vertices = new ofxVertex[m_VerticesSize];
	m_UVChange = true;
	LoadShader(DEFAULT_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	ofxSpriteBase::~ofxSpriteBase();
	ofxRENDERER->EraseSprite(this);
}
void ofxSpriteQuad::SetTexture(string texture_path)
{
	ofxSpriteBase::SetTexture(texture_path);
	SetSpriteRect(-m_Texture->GetTextureSize().x*0.5, 0, m_Texture->GetTextureSize().x, m_Texture->GetTextureSize().y);
	SetTextureRect(0, 0, m_Texture->GetTextureSize().x, m_Texture->GetTextureSize().y);
}
ofRectangle ofxSpriteQuad::GetTextureRect()
{
	return m_TextureRect;
}
ofRectangle ofxSpriteQuad::GetSpriteRect()
{
	return m_SpriteRect;
}
void ofxSpriteQuad::SetTextureRect(const float x, const float y, const float w, const float h)
{
	SetTextureRect(ofRectangle(x, y, w, h));
}
void ofxSpriteQuad::SetSpriteRect(const float x, const float y, const float w, const float h)
{
	SetSpriteRect(ofRectangle(x, y, w, h));
}
void ofxSpriteQuad::SetTextureRect(const ofRectangle rect)
{
	m_TextureRect = rect;
	m_UVChange = true;
}
void ofxSpriteQuad::SetSpriteRect(const ofRectangle rect)
{
	m_SpriteRect = rect;
	m_Dimension = m_SpriteRect;
	m_DimensionChange = true;
}
// in order to make the quad skew 30 degree, we must put some adjust on Y and Z
#define SKEW45
#if defined(SKEW30)
#define QUAD_GRADIENT_Z 0.4472135954999579// sqrt(0.2)
#define QUAD_GRADIENT_Y 0.8944271909999159// sqrt(0.8)
#elif defined(SKEW45)
#define QUAD_GRADIENT_Z 0.7071067811865475// sqrt(0.5)
#define QUAD_GRADIENT_Y 0.7071067811865475// sqrt(0.5)
#endif
void ofxSpriteQuad::SubmitChanges()
{
	ofxSpriteBase::SubmitChanges();
	if(m_PositionChange || m_DimensionChange)
	{
		m_Vertices[0].x = m_Position.x + m_SpriteRect.x;
		m_Vertices[0].y = m_Position.y + m_SpriteRect.y;
		m_Vertices[0].z = m_Position.z;
		m_Vertices[1].x = m_Vertices[0].x + m_SpriteRect.width;
		m_Vertices[1].y = m_Vertices[0].y;
		m_Vertices[1].z = m_Vertices[0].z;
		m_Vertices[2].x = m_Vertices[1].x;
		m_Vertices[2].y = m_Vertices[1].y + QUAD_GRADIENT_Y*m_SpriteRect.height;
		m_Vertices[2].z = m_Vertices[0].z - QUAD_GRADIENT_Z*m_SpriteRect.height;
		m_Vertices[3].x = m_Vertices[0].x;
		m_Vertices[3].y = m_Vertices[2].y;
		m_Vertices[3].z = m_Vertices[2].z;
	}
	if(m_UVChange)
	{
		float uv_min_x = m_TextureRect.x/m_Texture->GetTextureSize().x;
		float uv_min_y = m_TextureRect.y/m_Texture->GetTextureSize().y;
		float uv_max_x = uv_min_x + m_TextureRect.width/m_Texture->GetTextureSize().x;
		float uv_max_y = uv_min_y + m_TextureRect.height/m_Texture->GetTextureSize().y;
		swap(uv_min_y, uv_max_y);
		m_Vertices[0].u = uv_min_x;
		m_Vertices[0].v = uv_min_y;
		m_Vertices[1].u = uv_max_x;
		m_Vertices[1].v = uv_min_y;
		m_Vertices[2].u = uv_max_x;
		m_Vertices[2].v = uv_max_y;
		m_Vertices[3].u = uv_min_x;
		m_Vertices[3].v = uv_max_y;
	}
	m_PositionChange = m_DimensionChange = m_UVChange = false;
}
/* ----------------------------------
sprite operation
---------------------------------- */
void ofxSpriteQuad::SetScale(float value)
{
	m_ScaleX = m_ScaleY = value;
}
void ofxSpriteQuad::SetScaleX(float value)
{
	m_ScaleX = value;
}
float ofxSpriteQuad::GetScaleX()
{
	return m_ScaleX;
}
void ofxSpriteQuad::SetScaleY(float value)
{
	m_ScaleY = value;
}
float ofxSpriteQuad::GetScaleY()
{
	return m_ScaleY;
}
void ofxSpriteQuad::SetMirrorX(bool value)
{
	m_MirrorX = value;
}
bool ofxSpriteQuad::IsMirrorX()
{
	return m_MirrorX;
}
void ofxSpriteQuad::SetMirrorY(bool value)
{
	m_MirrorY = value;
}
bool ofxSpriteQuad::IsMirrorY()
{
	return m_MirrorY;
}
void ofxSpriteQuad::SetOpacity(unsigned char value)
{
	m_Opacity = value;
}
unsigned char ofxSpriteQuad::GetOpacity()
{
	return m_Opacity;
}
void ofxSpriteQuad::SetColor(ofColor value)
{
	m_Color = value;
}
ofColor ofxSpriteQuad::GetColor()
{
	return m_Color;
}
void ofxSpriteQuad::SetColorIntensity(unsigned char value)
{
	m_ColorIntensity = value;
}
unsigned char ofxSpriteQuad::GetColorIntensity()
{
	return m_ColorIntensity;
}
void ofxSpriteQuad::SetUVChange()
{
	m_UVChange = true;
}
bool ofxSpriteQuad::GetUVChange()
{
	return m_UVChange;
}