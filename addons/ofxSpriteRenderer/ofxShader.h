#pragma once
#include "ofMain.h"
#include "ofxResource.h"
class ofxShader
	:public ofxResource
{
	friend class ofxShaderProgram;
private:
	GLuint m_ShaderId;
public:
	ofxShader();
	~ofxShader();
	virtual bool Load(string shader_file);
	virtual void IncreaseReference();
	virtual void DecreaseReference();
	bool Load(string shader_file, GLenum type);
};
class ofxShaderProgram
{
public:
	static ofxShaderProgram* s_ActivatedShader;
private:
	int m_TextureCount;
	GLuint* m_TextureOrder;
private:
	ofxShader* m_VertexShader;
	ofxShader* m_FragmentShader;
private:
	GLuint m_ShaderProgramId;
	GLint m_ShaderLocationXYZ;
	GLint m_ShaderLocationUV;
	GLint m_ShaderLocationTextureCount;
	GLint* m_ShaderLocationTexture;
	GLint m_ShaderLocationProjection;
	GLint m_ShaderLocationModelView;
	GLint m_ShaderLocationTransform;
	GLint m_ShaderLocationInvModelView;
public:
	ofxShaderProgram();
	virtual ~ofxShaderProgram();
	virtual bool Load(string program_file);
	virtual void IncreaseReference();
	virtual void DecreaseReference();
	bool Load(string vs_file, string fs_file);
	void SetOrder(const int texture_id, const int order);
	void Bind();
	void Unbind();
	
};