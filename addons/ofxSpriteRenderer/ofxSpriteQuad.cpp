#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
ofxSpriteQuad::ofxSpriteQuad()
{
	m_TextureRect = 0;
	m_SpriteRect = 0;
	m_Material = 0;
	m_PositionUpdated = false;
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
void ofxSpriteQuad::MoveTo(const ofVec3f position)
{
	m_WorldPosition = position;
	m_PositionUpdated = false;
	m_DistanceUpdated = false;
	m_Status = QUAD_STATUS_POSITION_CHANGE;
	m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
}
void ofxSpriteQuad::MoveBy(const ofVec3f accelerator)
{
	m_WorldPosition += accelerator;
	m_PositionUpdated = false;
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
void ofxSpriteQuad::CalculatePosition(const ofVec3f camera_position)
{
	m_Position.x = m_WorldPosition.x + camera_position.x;
	m_Position.y = m_WorldPosition.y + camera_position.y;
	m_Position.x /= ofGetWindowWidth();
	m_Position.y /= ofGetWindowHeight();
	m_PositionUpdated = true;
	m_DistanceUpdated = false;
	m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
}
float ofxSpriteQuad::CalculateDistanceToCamera(const ofVec3f camera_position)
{
	return m_WorldPosition.z;
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
	if(m_Visibility == QUAD_VISIBILITY_FAR_SCREEN 
		&& ofGetFrameNum() % FAR_SCREEN_UPDATE_SEQUENCE != 0)
	{
		return;
	}
	if(!(IsPositionUpdated() && camera_updated))
	{
		if(m_Status == QUAD_STATUS_NO_CHANGE)
		{
			m_Status = QUAD_STATUS_SAFE_CHANGE;
		}
		ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
		CalculatePosition(camera_position);
	}
	if(m_Status == QUAD_STATUS_NO_CHANGE || m_Status == QUAD_STATUS_MATERIAL_CHANGE) return;
	float x_min = m_Position.x - GetWidth()*0.5;
	float x_max = x_min + GetWidth();
	float y_min = GetPosition().y;
	float y_max = y_min + GetHeight();

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
	else if(y_max < -1.0f)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_max < -1.0f)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(y_min > 1.0f)
	{
		m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_min > 1.0f)
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
	if(m_Status == QUAD_STATUS_NO_CHANGE) return;
	if(m_Visibility == QUAD_VISIBILITY_FAR_SCREEN) return;
	m_ParentCommand->UpdateSprite(this);
	m_Status = QUAD_STATUS_NO_CHANGE;
}
