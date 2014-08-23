#pragma once
#include "ofMain.h"
#include "ofxSpriteQuad.h"

enum COMMAND_STATUS
{
	COMMAND_STATUS_UNITED,
	COMMAND_STATUS_DISMISSED,
	COMMAND_STATUS_EXPANDED
};
#define COMMAND_CAPACITY 5000

class ofxSpriteCommand
{
	friend class ofxSpriteQuad;
	friend class ofxSpriteRenderer;
public:
	static void				GenerateSharedIndices();
	static void				DeleteSharedIndices();
	static GLuint*			m_Indices;
	static GLuint			m_IBOId;
private:
	ofxTexture*				m_Texture;
	ofxShaderProgram*		m_Shader;
	ofxVertex*				m_Vertices;
	GLuint					m_VBOId;
	GLsizei					m_VerticesSize;
	GLsizei					m_IndicesSize;
public:
	ofxSpriteCommand();
	~ofxSpriteCommand();
private:
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	ofxTexture* GetTexture();
	ofxShaderProgram* GetShader();
	void SetTexture(ofxTexture* texture);
	void SetShader(ofxShaderProgram* shader);
};
typedef vector<ofxSpriteCommand*> ofxSpriteCommands;