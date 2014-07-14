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
	virtual bool IsUnused();
	bool Load(string shader_file, GLenum type);
};