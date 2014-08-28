#pragma once
#include "ofMain.h"
#include "ofxSpriteQuad.h"

#define COMMAND_CAPACITY 5000

class ofxSpriteCommand
{
	friend class ofxSpriteQuad;
	friend class ofxSpriteRenderer;
private:
	ofxTexture*				m_Texture;
	ofxShaderProgram*		m_Shader;
	ofxVertex*				m_Vertices;
	GLuint					m_VBOId;
	GLsizei					m_VerticesSize;
public:
	ofxSpriteCommand();
	~ofxSpriteCommand();
private:
	void					Render();
	void					PushSprite(ofxSpriteQuad* sprite);
	ofxTexture*				GetTexture();
	ofxShaderProgram*		GetShader();
	void					SetTexture(ofxTexture* texture);
	void					SetShader(ofxShaderProgram* shader);
};
typedef vector<ofxSpriteCommand*> ofxSpriteCommands;