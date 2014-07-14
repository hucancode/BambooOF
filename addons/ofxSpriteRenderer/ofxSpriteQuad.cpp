#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
#include "ofxShaderProgramCache.h"
#include "ofxTextureCache.h"
ofxSpriteQuad::ofxSpriteQuad()
{
	m_Shader = 0;
	m_Visibility = QUAD_VISIBILITY_UNKNOWN;
	m_PositionChange = true;
	m_MaterialChange = true;
	m_UVChange = true;
	m_VisibilityChange = true;
	m_Transparent = true;
	m_UsePrivateShader = false;
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	if(m_Shader) m_Shader->DecreaseReference();
}
void ofxSpriteQuad::SetUsePrivateShader(bool value)
{
	m_UsePrivateShader = value;
}
bool ofxSpriteQuad::IsUsingPrivateShader()
{
	return m_UsePrivateShader;
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
	if(m_UsePrivateShader)
	{
		m_Shader = new ofxShaderProgram();
		m_Shader->Load(shader_path);
	}
	else
	{
		m_Shader = ofxSHADERPROGRAMCACHE->GetResource(shader_path);
	}
	m_Shader->IncreaseReference();
}
void ofxSpriteQuad::PushTexture(string texture_path)
{
	ofxTexture* texture = ofxTEXTURECACHE->GetResource(texture_path);
	texture->IncreaseReference();
	m_Textures.push_back(texture);
	m_TextureRect.push_back(ofVec4f::zero());
	m_SpriteRect.push_back(ofVec4f::zero());
	m_glUV.push_back(ofVec4f::zero());
	m_glCUV.push_back(ofVec4f::zero());
}
void ofxSpriteQuad::ClearTexture()
{
	ofxTextures::iterator it = m_Textures.begin();
	for(;it != m_Textures.end();it++)
	{
		(*it)->DecreaseReference();
	}
	m_Textures.clear();
}
ofxTextures ofxSpriteQuad::GetTextures()
{
	return m_Textures;
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
		m_Visibility = QUAD_VISIBILITY_UNKNOWN;
	}
	else if(accelerator.y > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_UNKNOWN;
	}
}
void ofxSpriteQuad::UpdateVisibility(bool force_update, bool camera_move)
{
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

	float x_min = m_WorldPosition.x - m_WorldQuad.x*0.5;
	float x_max = x_min + m_WorldQuad.x;
	float z_max = m_WorldPosition.z;
	float z_min = z_max - m_WorldQuad.y;
	
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
bool ofxSpriteQuad::IsTransparent()
{
	return m_Transparent;
}
void ofxSpriteQuad::SetTransparent(bool value)
{
	m_Transparent = value;
}
QUAD_VISIBILITY ofxSpriteQuad::GetVisibility()
{
	return m_Visibility;
}
float ofxSpriteQuad::GetLogicWidth()
{
	return m_WorldQuad.x;
}
void ofxSpriteQuad::SetLogicWidth(const float width)
{
	if(m_WorldQuad.x == width) return;
	m_WorldQuad.x = width;
	m_DimensionChange = true;
}
float ofxSpriteQuad::GetLogicHeight()
{
	return m_WorldQuad.y;
}
void ofxSpriteQuad::SetLogicHeight(const float height)
{
	if(m_WorldQuad.y == height) return;
	m_WorldQuad.y = height;
	m_DimensionChange = true;
}
void ofxSpriteQuad::SetLogicSize(const float width, const float height)
{
	SetLogicSize(ofVec2f(width,height));
}
void ofxSpriteQuad::SetLogicSize(ofVec2f dimension)
{
	if(m_WorldQuad == dimension) return;
	m_WorldQuad = dimension;
	m_DimensionChange = true;
}
void ofxSpriteQuad::SetTextureRect(const int index, const float x, const float y, const float w, const float h)
{
	SetTextureRect(index, ofVec4f(x,y,w,h));
}
void ofxSpriteQuad::SetSpriteRect(const int index, const float x, const float y, const float w, const float h)
{
	SetSpriteRect(index, ofVec4f(x,y,w,h));
}
void ofxSpriteQuad::SetTextureRect(const int index, const ofVec4f rect)
{
	m_TextureRect[index] = rect;
	m_UVChange = true;
}
void ofxSpriteQuad::SetSpriteRect(const int index, const ofVec4f rect)
{
	m_SpriteRect[index] = rect;
	m_UVChange = true;
}
#define QUAD_GRADIENT 0.7071067811865475
void ofxSpriteQuad::SubmitChanges()
{
	UpdateVisibility(ofxRENDERER->IsCameraForce(), ofxRENDERER->IsCameraMove());
	if(!(m_PositionChange || m_MaterialChange || m_UVChange || m_VisibilityChange)) return;
	if(m_Visibility == QUAD_VISIBILITY_FAR_SCREEN || m_Visibility == QUAD_VISIBILITY_OFF_SCREEN)
	{
		m_glPosition[0].x = -1000000.0;
		m_glPosition[0].y = -1000000.0;
		m_glPosition[0].z = -1000000.0;
		m_glPosition[1].x = -1000000.0;
		m_glPosition[1].y = -1000000.0;
		m_glPosition[1].z = -1000000.0;
		m_glPosition[2].x = -1000000.0;
		m_glPosition[2].y = -1000000.0;
		m_glPosition[2].z = -1000000.0;
		m_glPosition[3].x = -1000000.0;
		m_glPosition[3].y = -1000000.0;
		m_glPosition[3].z = -1000000.0;
	}
	else
	{
		if(m_PositionChange)
		{
			m_glPosition[0].x = m_WorldPosition.x - m_WorldQuad.x*0.5;
			m_glPosition[0].y = m_WorldPosition.y;
			m_glPosition[0].z = m_WorldPosition.z;
			m_glPosition[1].x = m_glPosition[0].x + m_WorldQuad.x;
			m_glPosition[1].y = m_glPosition[0].y;
			m_glPosition[1].z = m_glPosition[0].z;
			m_glPosition[2].x = m_glPosition[1].x;
			m_glPosition[2].y = m_glPosition[1].y + m_WorldQuad.y;
			m_glPosition[2].z = m_glPosition[0].z - QUAD_GRADIENT*m_WorldQuad.y;
			m_glPosition[3].x = m_glPosition[0].x;
			m_glPosition[3].y = m_glPosition[2].y;
			m_glPosition[3].z = m_glPosition[2].z;
		}
		// TODO: optimize these calculation will help gain alot of fps
		if(m_UVChange)
		{
			for(int i=0;i< m_Textures.size();i++)
			{
				float uv_w = m_WorldQuad.x/m_Textures[i]->GetTextureSize().x;
				float uv_h = m_WorldQuad.y/m_Textures[i]->GetTextureSize().y;
				float uv_min_x = (m_TextureRect[i].x - m_SpriteRect[i].x)/m_Textures[i]->GetTextureSize().x;
				float uv_min_y = (m_TextureRect[i].y - m_SpriteRect[i].y)/m_Textures[i]->GetTextureSize().y;
				float uv_max_x = uv_min_x + uv_w;
				float uv_max_y = uv_min_y + uv_h;
				m_glUV[i].x = uv_min_x;
				m_glUV[i].y = uv_max_y;
				m_glUV[i].z = uv_max_x;
				m_glUV[i].w = uv_min_y;
				float crop_w = m_WorldQuad.x/m_SpriteRect[i].z;
				float crop_h = m_WorldQuad.y/m_SpriteRect[i].w;
				float crop_min_x = -m_SpriteRect[i].x/m_SpriteRect[i].z;
				float crop_min_y = -m_SpriteRect[i].y/m_SpriteRect[i].w;
				float crop_max_x = crop_min_x + crop_w;
				float crop_max_y = crop_min_y + crop_h;
				m_glCUV[i].x = crop_min_x;
				m_glCUV[i].y = crop_max_y;
				m_glCUV[i].z = crop_max_x;
				m_glCUV[i].w = crop_min_y;
			}
		}
	}
	m_ParentCommand->UpdateSprite(this);
	m_PositionChange = false;
	m_MaterialChange = false;
	m_UVChange = false;
	m_VisibilityChange = false;
}
