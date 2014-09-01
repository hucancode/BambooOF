#include "ofxSpriteBase.h"
#include "ofxSpriteRenderer.h"
void ofxSpriteBase::MoveTo(const float x, const float y, const float z)
{
	MoveTo(ofVec3f(x,y,z));
}
void ofxSpriteBase::MoveTo(const ofVec3f position)
{
	m_WorldPosition = position;
	m_PositionChange = true;
	m_Visibility = QUAD_VISIBILITY_UNKNOWN;
}
void ofxSpriteBase::MoveBy(const float x, const float y, const float z)
{
	MoveBy(ofVec3f(x,y,z));
}
void ofxSpriteBase::MoveBy(const ofVec3f accelerator)
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
bool ofxSpriteBase::IsBehind(ofxSpriteBase* other)
{
	return this->GetWorldPosition().z < other->GetWorldPosition().z;
}
bool ofxSpriteBase::IsInFront(ofxSpriteBase* other)
{
	return this->GetWorldPosition().z > other->GetWorldPosition().z;
}
void ofxSpriteBase::SetVisible(bool value)
{
	m_Visible = value;
}
bool ofxSpriteBase::IsVisible()
{
	return m_Visible;
}
QUAD_VISIBILITY	ofxSpriteBase::GetVisibility()
{
	return m_Visibility;
}
ofVec3f ofxSpriteBase::GetWorldPosition()
{
	return m_WorldPosition;
}