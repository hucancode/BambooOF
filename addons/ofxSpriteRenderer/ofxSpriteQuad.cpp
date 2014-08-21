#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
#include "ofxShaderProgramCache.h"
#include "ofxTextureCache.h"
ofxSpriteQuad::ofxSpriteQuad()
{
	m_Shader = 0;
	m_Visibility = QUAD_VISIBILITY_UNKNOWN;
	m_Texture = 0;
	m_PositionChange = true;
	m_DimensionChange = true;
	m_UVChange = true;
	m_VisibilityChange = true;
	LoadShader(DEFAULT_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	if(m_Shader) m_Shader->DecreaseReference();
	ofxRENDERER->EraseSprite(this);
}
void ofxSpriteQuad::SetVisible(bool value)
{
	m_Visible = value;
}
bool ofxSpriteQuad::IsVisible()
{
	return m_Visible;
}
ofxShaderProgram* ofxSpriteQuad::GetShader()
{
	return m_Shader;
}
void ofxSpriteQuad::LoadShader(string shader_path)
{
	if(m_Shader)
	{
		m_Shader->DecreaseReference();
	}
	m_Shader = ofxSHADERPROGRAMCACHE->GetResource(shader_path);
	m_Shader->IncreaseReference();
}
void ofxSpriteQuad::SetTexture(string texture_path)
{
	if(m_Texture)
	{
		m_Texture->DecreaseReference();
	}
	m_Texture = ofxTEXTURECACHE->GetResource(texture_path);
	m_Texture->IncreaseReference();
	SetSpriteRect(0, 0, m_Texture->GetTextureSize().x, m_Texture->GetTextureSize().y);
	SetTextureRect(0, 0, m_Texture->GetTextureSize().x, m_Texture->GetTextureSize().y);
}
ofxTexture* ofxSpriteQuad::GetTexture()
{
	return m_Texture;
}
void ofxSpriteQuad::MoveTo(const float x, const float y, const float z)
{
	MoveTo(ofVec3f(x,y,z));
}
void ofxSpriteQuad::MoveTo(const ofVec3f position)
{
	m_WorldPosition = position;
	m_PositionChange = true;
	m_Visibility = QUAD_VISIBILITY_UNKNOWN;
}
void ofxSpriteQuad::MoveBy(const float x, const float y, const float z)
{
	MoveBy(ofVec3f(x,y,z));
}
void ofxSpriteQuad::MoveBy(const ofVec3f accelerator)
{
	m_WorldPosition += accelerator;
	m_PositionChange = true;
	if(accelerator.x > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
	else if(accelerator.y > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
}
void ofxSpriteQuad::UpdateVisibility(bool force_update, bool camera_move)
{
	/*m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	return;*/
	// if camera isn't looking straight into world, these calculation mean nothing
	if(!(camera_move || m_PositionChange))
		return;
	if(!force_update && 
		(m_Visibility == QUAD_VISIBILITY_FAR_SCREEN && ofGetFrameNum() % FAR_SCREEN_UPDATE_SEQUENCE != 0))
		return;
	if(!force_update && 
		(m_Visibility == QUAD_VISIBILITY_IN_SCREEN && ofGetFrameNum() % IN_SCREEN_UPDATE_SEQUENCE != 0))
		return;

	QUAD_VISIBILITY old_visibility = m_Visibility;

	float x_min = m_WorldPosition.x - m_SpriteRect.z*0.5 + m_SpriteRect.x;
	float x_max = x_min + m_SpriteRect.z;
	float z_max = m_WorldPosition.z;
	float z_min = z_max - m_SpriteRect.w;
	
	if(z_max < -FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(x_max < -FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(z_min > FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(x_min > FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(z_max < ofxRENDERER->GetWorldRect().y)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_max < ofxRENDERER->GetWorldRect().x)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(z_min > ofxRENDERER->GetWorldRect().w)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_min > ofxRENDERER->GetWorldRect().z)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else
	{
		m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
	if(m_Visibility != old_visibility)
	{
		m_VisibilityChange = true;
	}
}

ofVec3f ofxSpriteQuad::GetWorldPosition()
{
	return m_WorldPosition;
}
QUAD_VISIBILITY ofxSpriteQuad::GetVisibility()
{
	return m_Visibility;
}
ofVec4f ofxSpriteQuad::GetTextureRect()
{
	return m_TextureRect;
}
ofVec4f ofxSpriteQuad::GetSpriteRect()
{
	return m_SpriteRect;
}
void ofxSpriteQuad::SetTextureRect(const float x, const float y, const float w, const float h)
{
	m_TextureRect.set(x,y,w,h);
	m_UVChange = true;
}
void ofxSpriteQuad::SetSpriteRect(const float x, const float y, const float w, const float h)
{
	m_SpriteRect.set(x,y,w,h);
	m_DimensionChange = true;
}
void ofxSpriteQuad::SetTextureRect(const ofVec4f rect)
{
	m_TextureRect = rect;
	m_UVChange = true;
}
void ofxSpriteQuad::SetSpriteRect(const ofVec4f rect)
{
	m_SpriteRect = rect;
	m_DimensionChange = true;
}
void ofxSpriteQuad::SetID(int id)
{
	m_ID = id;
}
int ofxSpriteQuad::GetID()
{
	return m_ID;
}
// in order to make the quad skew 30 degree, we must put some adjust on Y and Z
#define SKEW30
#if defined(SKEW30)
#define QUAD_GRADIENT_Z 0.4472135954999579// sqrt(0.2)
#define QUAD_GRADIENT_Y 0.8944271909999159// sqrt(0.8)
#elif defined(SKEW45)
#define QUAD_GRADIENT_Z 0.7071067811865475// sqrt(0.5)
#define QUAD_GRADIENT_Y 0.7071067811865475// sqrt(0.5)
#endif
void ofxSpriteQuad::SubmitChanges()
{
	UpdateVisibility(ofxRENDERER->IsCameraForce(), ofxRENDERER->IsCameraMove());
	if(!(m_PositionChange || m_DimensionChange || m_UVChange || m_VisibilityChange)) return;
	if(m_PositionChange || m_DimensionChange)
	{
		m_Vertices[0].x = m_WorldPosition.x - m_SpriteRect.z*0.5 + m_SpriteRect.x;
		m_Vertices[0].y = m_WorldPosition.y + m_SpriteRect.y;
		m_Vertices[0].z = m_WorldPosition.z;
		m_Vertices[1].x = m_Vertices[0].x + m_SpriteRect.z;
		m_Vertices[1].y = m_Vertices[0].y;
		m_Vertices[1].z = m_Vertices[0].z;
		m_Vertices[2].x = m_Vertices[1].x;
		m_Vertices[2].y = m_Vertices[1].y + QUAD_GRADIENT_Y*m_SpriteRect.w;
		m_Vertices[2].z = m_Vertices[0].z - QUAD_GRADIENT_Z*m_SpriteRect.w;
		m_Vertices[3].x = m_Vertices[0].x;
		m_Vertices[3].y = m_Vertices[2].y;
		m_Vertices[3].z = m_Vertices[2].z;
	}
	if(m_UVChange)
	{
		float uv_min_x = m_TextureRect.x/m_Texture->GetTextureSize().x;
		float uv_min_y = m_TextureRect.y/m_Texture->GetTextureSize().y;
		float uv_max_x = uv_min_x + m_TextureRect.z/m_Texture->GetTextureSize().x;
		float uv_max_y = uv_min_y + m_TextureRect.w/m_Texture->GetTextureSize().y;
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
	m_PositionChange = m_DimensionChange = m_UVChange = m_VisibilityChange = false;
}
bool ofxSpriteQuad::IsBehind(ofxSpriteQuad* other)
{
	return this->GetWorldPosition().z < other->GetWorldPosition().z;
}
bool ofxSpriteQuad::IsInFront(ofxSpriteQuad* other)
{
	return this->GetWorldPosition().z > other->GetWorldPosition().z;
}