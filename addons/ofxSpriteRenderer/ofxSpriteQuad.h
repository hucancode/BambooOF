#pragma once
#include "ofMain.h"
#include "ofxShaderProgram.h"
#include "ofxTexture.h"
struct ofxVertex
{
	float X,Y,Z;// position
	float UV[60];// texture uv
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
	ofxTextures			m_Textures;
	ofxShaderProgram*	m_Shader;
	bool				m_UsePrivateShader;
	unsigned int		m_IndexInCommand;
	ofxSpriteCommand*	m_ParentCommand;
public:
	unsigned int		m_IndexInRenderer;
	ofxSpriteQuad*		m_NextSibling;
	ofxSpriteQuad*		m_PrevSibling;
protected:
	bool				m_PositionChange;
	bool				m_DimensionChange;
	bool				m_MaterialChange;
	bool				m_UVChange;
	bool				m_VisibilityChange;
protected:
	QUAD_VISIBILITY		m_Visibility;
	ofVec3f				m_WorldPosition;
	ofVec2f				m_WorldQuad;
	bool				m_Visible;
protected:
	vector<ofVec4f>		m_TextureRect;
	vector<ofVec4f>		m_SpriteRect;
protected:
	ofVec3f				m_glPosition[4];
	vector<ofVec4f>		m_glUV;
	vector<ofVec4f>		m_glCUV;
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	virtual void		Update(const float delta_time){}
	void				MoveTo(const float x, const float y, const float z);
	void				MoveTo(const ofVec3f position);
	void				MoveBy(const float x, const float y, const float z);
	void				MoveBy(const ofVec3f accelerator);
	void				UpdateVisibility(bool force_update=false, bool camera_move=false);
	QUAD_VISIBILITY		GetVisibility();
	ofVec3f				GetWorldPosition();
	float				GetLogicWidth();
	void				SetLogicWidth(const float width);
	float				GetLogicHeight();
	void				SetLogicHeight(const float height);
	void				SetLogicSize(const float width, const float height);
	void				SetLogicSize(ofVec2f dimension);
public:
	void				SetUsePrivateShader(bool value);
	bool				IsUsingPrivateShader();
	ofxShaderProgram*	GetShader();
	void				LoadShader(string shader_path);
	void				PushTexture(string texture_path);
	void				ClearTexture();
	ofxTextures			GetTextures();
public:
	ofVec4f				GetTextureRect(const int index);
	ofVec4f				GetSpriteRect(const int index);
	void				SetTextureRect(const int index, const float x, const float y, const float w, const float h);
	void				SetSpriteRect(const int index, const float x, const float y, const float w, const float h);
	void				SetTextureRect(const int index, const ofVec4f rect);
	void				SetSpriteRect(const int index, const ofVec4f rect);
	void				SubmitChanges();
public:
	ofxSpriteQuad*		NextSibling();
	ofxSpriteQuad*		PreviousSibling();
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;