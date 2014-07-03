#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
ofxSpriteQuad::ofxSpriteQuad()
{
	m_TextureRect = 0;
	m_SpriteRect = 0;
	m_Material = 0;
	m_DistanceUpdated = false;
	m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	m_Status = QUAD_STATUS_NO_CHANGE;
	m_Transparent = true;
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	if(m_Material) m_Material->DecreaseReference();
	if(m_TextureRect) delete[] m_TextureRect;
	if(m_SpriteRect) delete[] m_SpriteRect;
	if(m_glUV) delete[] m_glUV;
	if(m_glCUV) delete[] m_glCUV;
}
void ofxSpriteQuad::SetMaterial(ofxSpriteMaterial* material)
{
	if(m_Material)
	{
		if(material == m_Material) return;
		m_Material->DecreaseReference();
	}
	m_Material = material;
	SetMaxTexture(material->GetTextureCount());
	m_Material->IncreaseReference();
	if(m_Status != QUAD_STATUS_POSITION_CHANGE)
	{
		m_Status = QUAD_STATUS_MATERIAL_CHANGE;
	}
}
void ofxSpriteQuad::MoveTo(const float x, const float y, const float z)
{
	MoveTo(ofVec3f(x,y,z));
}
void ofxSpriteQuad::MoveTo(const ofVec3f position)
{
	m_WorldPosition = position;
	m_DistanceUpdated = false;
	m_Status = QUAD_STATUS_POSITION_CHANGE;
	m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
}
void ofxSpriteQuad::MoveBy(const float x, const float y, const float z)
{
	MoveBy(ofVec3f(x,y,z));
}
void ofxSpriteQuad::MoveBy(const ofVec3f accelerator)
{
	m_WorldPosition += accelerator;
	m_DistanceUpdated = false;
	m_Status = QUAD_STATUS_POSITION_CHANGE;
	if(accelerator.x > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
	else if(accelerator.y > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
}
float ofxSpriteQuad::CalculateDistanceToCamera(const ofVec3f camera_position)
{
	return -m_WorldPosition.z;
	// legacy way
	if(!m_DistanceUpdated)
	{
		m_DistanceToCamera = camera_position.distance(m_WorldPosition);
		m_DistanceUpdated = true;
	}
	return m_DistanceToCamera;
}
void ofxSpriteQuad::UpdateVisibility(bool camera_updated)
{
	// if camera isn't looking straight into world, these calculation mean nothing
	if(m_Status == QUAD_STATUS_NO_CHANGE || m_Status == QUAD_STATUS_MATERIAL_CHANGE) return;
	if(m_Visibility == QUAD_VISIBILITY_FAR_SCREEN && ofGetFrameNum() % FAR_SCREEN_UPDATE_SEQUENCE != 0) 
		return;
	
	float x_min = m_WorldPosition.x - m_WorldQuad.x*0.5;
	float x_max = x_min + m_WorldQuad.x;
	float y_min = m_WorldPosition.y;
	float y_max = y_min + m_WorldQuad.y;
	
	if(y_max < -FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(x_max < -FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(y_min > FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(x_min > FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(y_max < ofxRENDERER->GetRenderRect().y)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_max < ofxRENDERER->GetRenderRect().x)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(y_min > ofxRENDERER->GetRenderRect().w)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_min > ofxRENDERER->GetRenderRect().z)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else
	{
		m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
}
void ofxSpriteQuad::SetMaxTexture(const int size)
{
	if(m_TextureRect) delete[] m_TextureRect;
	if(m_SpriteRect) delete[] m_SpriteRect;
	if(m_glUV) delete[] m_SpriteRect;
	if(m_glCUV) delete[] m_SpriteRect;
	m_TextureRect = new ofVec4f[size];
	m_SpriteRect = new ofVec4f[size];
	m_glUV = new ofVec4f[size];
	m_glCUV = new ofVec4f[size];
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
	if(m_Status == QUAD_STATUS_NO_CHANGE)
	{
		m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
}
void ofxSpriteQuad::SetSpriteRect(const int index, const ofVec4f rect)
{
	m_SpriteRect[index] = rect;
	if(m_Status == QUAD_STATUS_NO_CHANGE)
	{
		m_Status = QUAD_STATUS_SAFE_CHANGE;
	}
}
void ofxSpriteQuad::SubmitChanges()
{
	if(m_Status == QUAD_STATUS_NO_CHANGE) return;
	if(m_Visibility == QUAD_VISIBILITY_FAR_SCREEN) return;
	m_glPosition[0].x = m_WorldPosition.x - m_WorldQuad.x*0.5;
	m_glPosition[0].y = m_WorldPosition.y;
	m_glPosition[0].z = m_WorldPosition.z;
	m_glPosition[1].x = m_glPosition[0].x + m_WorldQuad.x;
	m_glPosition[1].y = m_glPosition[0].y;
	m_glPosition[1].z = m_glPosition[0].z;
	m_glPosition[2].x = m_glPosition[1].x;
	m_glPosition[2].y = m_glPosition[1].y + m_WorldQuad.y;
	m_glPosition[2].z = m_glPosition[0].z;
	m_glPosition[3].x = m_glPosition[0].x;
	m_glPosition[3].y = m_glPosition[2].y;
	m_glPosition[3].z = m_glPosition[0].z;

	for(int i=0;i<m_Material->GetTextureCount();i++)
	{
		float uv_w = m_Material->GetTextureSize(i).x/m_WorldQuad.x;
		float uv_h = m_Material->GetTextureSize(i).y/m_WorldQuad.y;
		float uv_min_x = (m_TextureRect[i].x - m_SpriteRect[i].x)/uv_w;
		float uv_min_y = (m_TextureRect[i].y - m_SpriteRect[i].y)/uv_h;
		float uv_max_x = uv_min_x + uv_w;
		float uv_max_y = uv_min_y + uv_h;
		m_glUV[i].x = uv_min_x;
		m_glUV[i].y = uv_min_y;
		m_glUV[i].z = uv_max_x;
		m_glUV[i].w = uv_max_y;
		float crop_w = m_WorldQuad.x/m_SpriteRect[i].z;
		float crop_h = m_WorldQuad.y/m_SpriteRect[i].w;
		float crop_min_x = -m_SpriteRect[i].x/m_SpriteRect[i].z;
		float crop_min_y = -m_SpriteRect[i].x/m_SpriteRect[i].w;
		float crop_max_x = crop_min_x + crop_w;
		float crop_max_y = crop_min_y + crop_h;
		m_glCUV[i].x = crop_min_x;
		m_glCUV[i].y = crop_min_y;
		m_glCUV[i].z = crop_max_x;
		m_glCUV[i].w = crop_max_y;
	}
	m_ParentCommand->UpdateSprite(this);
	m_Status = QUAD_STATUS_NO_CHANGE;
}
