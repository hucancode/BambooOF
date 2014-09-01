#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"
#include "ofxShaderProgram.h"
#include "ofxTexture.h"

class ofxSpriteCommand;
class ofxSpriteRenderer;
class ofxSpriteQuad : public ofxSpriteBase
{
	friend ofxSpriteCommand;
	friend ofxSpriteRenderer;
protected:
	ofxTexture*			m_Texture;
	ofxShaderProgram*	m_Shader;
	int					m_ID;
protected:
	bool				m_DimensionChange;
	bool				m_UVChange;
	bool				m_VisibilityChange;
protected:
	ofVec4f				m_TextureRect;
	ofVec4f				m_SpriteRect;
protected:
	ofxVertex			m_Vertices[4];
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	virtual void		Update(const float delta_time){}
	void				SetTextureRect(const float x, const float y, const float w, const float h);
	void				SetTextureRect(const ofVec4f rect);
	void				SetSpriteRect(const float x, const float y, const float w, const float h);
	void				SetSpriteRect(const ofVec4f rect);
	ofVec4f				GetTextureRect();
	ofVec4f				GetSpriteRect();
	void				SubmitChanges();
	int					GetID();
public:
	ofxShaderProgram*	GetShader();
	void				LoadShader(string shader_path);
	void				SetTexture(string texture_path);
	ofxTexture*			GetTexture();
private:
	void				UpdateVisibility(bool force_update=false, bool camera_move=false);
	void				SetID(int id);
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;
#define DEFAULT_SHADER "sprite2d"