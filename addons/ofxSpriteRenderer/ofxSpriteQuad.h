#pragma once
#include "ofMain.h"
#include "ofxShaderProgram.h"
#include "ofxTexture.h"
struct ofxVertex
{
	float x,y,z;// position
	float u,v;// texture uv
};
enum QUAD_VISIBILITY
{
	QUAD_VISIBILITY_IN_SCREEN,
	QUAD_VISIBILITY_OFF_SCREEN,
	QUAD_VISIBILITY_FAR_SCREEN,
	QUAD_VISIBILITY_UNKNOWN,
};

class ofxSpriteCommand;
class ofxSpriteRenderer;
class ofxSpriteQuad
{
	friend ofxSpriteCommand;
	friend ofxSpriteRenderer;
protected:
	ofxTexture*			m_Texture;
	ofxShaderProgram*	m_Shader;
	int					m_ID;
protected:
	bool				m_PositionChange;
	bool				m_DimensionChange;
	bool				m_UVChange;
	bool				m_VisibilityChange;
protected:
	QUAD_VISIBILITY		m_Visibility;
	ofVec3f				m_WorldPosition;
	bool				m_Visible;
protected:
	ofVec4f				m_TextureRect;
	ofVec4f				m_SpriteRect;
protected:
	ofxVertex			m_Vertices[4];
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	virtual void		Update(const float delta_time){}
	void				SetVisible(bool value);
	bool				GetVisible();
	void				MoveTo(const float x, const float y, const float z);
	void				MoveTo(const ofVec3f position);
	void				MoveBy(const float x, const float y, const float z);
	void				MoveBy(const ofVec3f accelerator);
	void				SetTextureRect(const float x, const float y, const float w, const float h);
	void				SetTextureRect(const ofVec4f rect);
	void				SetSpriteRect(const float x, const float y, const float w, const float h);
	void				SetSpriteRect(const ofVec4f rect);
	ofVec4f				GetTextureRect();
	ofVec4f				GetSpriteRect();
	void				SubmitChanges();
	bool				IsBehind(ofxSpriteQuad* other);
	bool				IsInFront(ofxSpriteQuad* other);
	int					GetID();
public:
	QUAD_VISIBILITY		GetVisibility();
	ofVec3f				GetWorldPosition();
	ofxShaderProgram*	GetShader();
	void				LoadShader(string shader_path);
	void				SetTexture(string texture_path);
	ofxTexture*			GetTexture();
private:
	void				UpdateVisibility(bool force_update=false, bool camera_move=false);
	void				SetID(int id);
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;