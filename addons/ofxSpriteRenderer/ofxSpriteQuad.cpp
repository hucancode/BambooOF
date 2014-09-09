#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"

ofxSpriteQuad::ofxSpriteQuad()
{
	ofxSpriteBase::ofxSpriteBase();
	m_VerticesSize = 4;
	
	m_Vertices = new ofxVertex[m_VerticesSize];
	m_UVChange = true;
	m_MirrorX = m_MirrorY = false;
	m_ScaleX = m_ScaleY = 1.0;
	m_Angle = 0;
	m_Opacity = 255;
	m_ColorIntensity = 0;
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
	m_VertexAngle[0] = rect.getTopLeft().angle(ofVec2f::zero());
	m_VertexAngle[1] = rect.getTopRight().angle(ofVec2f::zero());
	m_VertexAngle[2] = rect.getBottomRight().angle(ofVec2f::zero());
	m_VertexAngle[3] = rect.getTopRight().angle(ofVec2f::zero());
	m_VertexDistance[0] = rect.getTopLeft().length();
	m_VertexDistance[1] = rect.getTopRight().length();
	m_VertexDistance[2] = rect.getBottomRight().length();
	m_VertexDistance[3] = rect.getTopRight().length();
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
		ofRectangle rect(m_SpriteRect);
		rect.x *= m_ScaleX;
		rect.y *= m_ScaleY;
		rect.width *= m_ScaleX;
		rect.height *= m_ScaleY;
		m_Vertices[0].x = rect.x + m_Position.x;
		m_Vertices[0].y = rect.y + m_Position.y;
		m_Vertices[0].z = m_Position.z;
		m_Vertices[1].x = m_Vertices[0].x + rect.width;
		m_Vertices[1].y = m_Vertices[0].y;
		m_Vertices[1].z = m_Vertices[0].z;
		m_Vertices[2].x = m_Vertices[1].x;
		m_Vertices[2].y = m_Vertices[1].y + QUAD_GRADIENT_Y*rect.height;
		m_Vertices[2].z = m_Vertices[0].z - QUAD_GRADIENT_Z*rect.height;
		m_Vertices[3].x = m_Vertices[0].x;
		m_Vertices[3].y = m_Vertices[2].y;
		m_Vertices[3].z = m_Vertices[2].z;
		if(m_Angle != 0)
		{
			m_Vertices[0].x += m_RotationEffect[0].x;
			m_Vertices[0].y += m_RotationEffect[0].y;
			m_Vertices[1].x += m_RotationEffect[1].x;
			m_Vertices[1].y += m_RotationEffect[1].y;
			m_Vertices[2].x += m_RotationEffect[2].x;
			m_Vertices[2].y += m_RotationEffect[2].y;
			m_Vertices[3].x += m_RotationEffect[3].x;
			m_Vertices[3].y += m_RotationEffect[3].y;
		}
	}
	if(m_UVChange)
	{
		float uv_min_x = m_TextureRect.x/m_Texture->GetTextureSize().x;
		float uv_min_y = m_TextureRect.y/m_Texture->GetTextureSize().y;
		float uv_max_x = uv_min_x + m_TextureRect.width/m_Texture->GetTextureSize().x;
		float uv_max_y = uv_min_y + m_TextureRect.height/m_Texture->GetTextureSize().y;
		swap(uv_min_y, uv_max_y);

		if(m_MirrorX)
		{
			swap(uv_max_x, uv_min_x);
		}
		if(m_MirrorY)
		{
			swap(uv_max_y, uv_min_y);
		}

		m_Vertices[0].u = uv_min_x;
		m_Vertices[0].v = uv_min_y;
		m_Vertices[1].u = uv_max_x;
		m_Vertices[1].v = uv_min_y;
		m_Vertices[2].u = uv_max_x;
		m_Vertices[2].v = uv_max_y;
		m_Vertices[3].u = uv_min_x;
		m_Vertices[3].v = uv_max_y;
	}
	if((m_AngleChange || m_DimensionChange) && m_Angle != 0)
	{
		m_RotationEffect[0].x = m_VertexDistance[0] * m_ScaleX * (GetCos(m_VertexAngle[0]) - GetCos(m_VertexAngle[0] + m_Angle));
		m_RotationEffect[0].y = m_VertexDistance[0] * m_ScaleY * (GetSin(m_VertexAngle[0]) - GetSin(m_VertexAngle[0] + m_Angle));
		m_RotationEffect[1].x = m_VertexDistance[1] * m_ScaleX * (GetCos(m_VertexAngle[1]) - GetCos(m_VertexAngle[1] + m_Angle));
		m_RotationEffect[1].y = m_VertexDistance[1] * m_ScaleY * (GetSin(m_VertexAngle[1]) - GetSin(m_VertexAngle[1] + m_Angle));
		m_RotationEffect[2].x = m_VertexDistance[2] * m_ScaleX * (GetCos(m_VertexAngle[2]) - GetCos(m_VertexAngle[2] + m_Angle));
		m_RotationEffect[2].y = m_VertexDistance[2] * m_ScaleY * (GetSin(m_VertexAngle[2]) - GetSin(m_VertexAngle[2] + m_Angle));
		m_RotationEffect[3].x = m_VertexDistance[3] * m_ScaleX * (GetCos(m_VertexAngle[3]) - GetCos(m_VertexAngle[3] + m_Angle));
		m_RotationEffect[3].y = m_VertexDistance[3] * m_ScaleY * (GetSin(m_VertexAngle[3]) - GetSin(m_VertexAngle[3] + m_Angle));
	}
	m_PositionChange = m_DimensionChange = m_UVChange = m_AngleChange = false;
}
/* ----------------------------------
sprite operation
---------------------------------- */
void ofxSpriteQuad::SetScale(float value)
{
	SetScaleX(value);
	SetScaleY(value);
}
void ofxSpriteQuad::SetScaleX(float value)
{
	if(m_ScaleX == value) return;
	m_ScaleX = value;
	m_DimensionChange = true;
}
float ofxSpriteQuad::GetScaleX()
{
	return m_ScaleX;
}
void ofxSpriteQuad::SetScaleY(float value)
{
	if(m_ScaleY == value) return;
	m_ScaleY = value;
	m_DimensionChange = true;
}
float ofxSpriteQuad::GetScaleY()
{
	return m_ScaleY;
}
void ofxSpriteQuad::SetAngle(int value)
{
	while(value >= 360) value -= 360;
	while(value < 0) value += 360;
	if(value == m_Angle) return;
	m_Angle = value;
	m_AngleChange = true;
}
int ofxSpriteQuad::GetAngle()
{
	return m_Angle;
}
void ofxSpriteQuad::SetMirrorX(bool value)
{
	m_MirrorX = value;
	m_UVChange = true;
}
bool ofxSpriteQuad::IsMirrorX()
{
	return m_MirrorX;
}
void ofxSpriteQuad::SetMirrorY(bool value)
{
	m_MirrorY = value;
	m_UVChange = true;
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
float ofxSpriteQuad::m_CosTable[360];
float ofxSpriteQuad::m_SinTable[360];
void ofxSpriteQuad::BuildSinCosTable()
{
	float deg_to_rad = PI/180.0;
	for(int i=0;i<360;i++)
	{
		float rad = i*deg_to_rad;
		m_CosTable[i] = cos(rad);
		m_SinTable[i] = sin(rad);
	}
}
float ofxSpriteQuad::GetCos(int degree)
{
	return m_CosTable[degree];
}
float ofxSpriteQuad::GetSin(int degree)
{
	return m_SinTable[degree];
}