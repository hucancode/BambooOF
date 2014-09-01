#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"
#include "ofxShaderProgram.h"
#include "ofxTexture.h"

struct ofxFont
{

};
class ofxSpriteText
{
private:
	ofxTexture*			m_Texture;
	ofxShaderProgram*	m_Shader;
	int					m_ID;
	string				m_Text;
	vector<ofxVertex>	m_Vertices;
public:
	ofxShaderProgram*	GetShader();
	void				LoadShader(string shader_path);
	void				SetTexture(string texture_path);
	ofxTexture*			GetTexture();
private:
	void				UpdateVisibility(bool force_update=false, bool camera_move=false);
	void				SetID(int id);
	int					GetID();
	void				SetText(string text);
	void				BuildMesh();
	void				SubmitChanges();
};
#define DEFAULT_TEXT_SHADER "sprite2d"