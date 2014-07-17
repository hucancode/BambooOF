#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxShader.h"

#define DEFAULT_TEXTURE_COUNT 15
class ofxShaderProgram
	:public ofxResource
{
private:
	vector<GLuint> m_TextureOrder;
private:
	ofxShader* m_VertexShader;
	ofxShader* m_FragmentShader;
private:
	GLuint m_ShaderProgramId;
	GLint m_ShaderLocationXYZ;
	vector<GLint> m_ShaderLocationUV;
	GLint m_ShaderLocationTextureCount;
	vector<GLint> m_ShaderLocationTexture;
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
	virtual bool IsUnused();
	bool Load(string vs_file, string fs_file);
	void SetOrder(const int texture_id, const int order);
	void Bind(const int texture_count=DEFAULT_TEXTURE_COUNT);
	void Unbind();
	
};