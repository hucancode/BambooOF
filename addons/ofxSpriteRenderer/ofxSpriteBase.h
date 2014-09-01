#pragma once
#include "ofMain.h"
struct ofxVertex
{
	float x,y,z;// position
	float u,v;// texture uv
};
// --------------------------------
// note:
// programmatically, QUAD_VISIBILITY_IN_SCREEN and QUAD_VISIBILITY_UNKNOWN define same state
// both needs to update position information. but we split it to improve code readability
// ---------------------------------
enum QUAD_VISIBILITY
{
	QUAD_VISIBILITY_IN_SCREEN,
	QUAD_VISIBILITY_OFF_SCREEN,
	QUAD_VISIBILITY_FAR_SCREEN,
	QUAD_VISIBILITY_UNKNOWN,
};
class ofxSpriteBase
{
protected:
	QUAD_VISIBILITY		m_Visibility;
	ofVec3f				m_WorldPosition;
	bool				m_Visible;
	bool				m_PositionChange;
public:
	QUAD_VISIBILITY		GetVisibility();
	ofVec3f				GetWorldPosition();
	void				SetVisible(bool value);
	bool				IsVisible();
	void				MoveTo(const float x, const float y, const float z);
	void				MoveTo(const ofVec3f position);
	void				MoveBy(const float x, const float y, const float z);
	void				MoveBy(const ofVec3f accelerator);
	bool				IsBehind(ofxSpriteBase* other);
	bool				IsInFront(ofxSpriteBase* other);
};
typedef vector<ofxSpriteBase*> ofxSpriteBases;