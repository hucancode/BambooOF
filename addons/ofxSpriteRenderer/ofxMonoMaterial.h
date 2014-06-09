#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
class ofxMonoMaterial
	:public ofxSpriteMaterial
{
private:
	unsigned int m_ReferenceCount;
private:
	GLuint m_TextureId;
	GLuint m_TextureSize[2];
	GLuint m_ShaderProgramId;
	GLint m_ShaderLocationXYZ;
	GLint m_ShaderLocationUV;
	GLint m_ShaderLocationCUV;
public:
	ofxMonoMaterial();
	~ofxMonoMaterial();
	void LoadTexturePNG(const char* texture_file);
	virtual bool LoadShader(const char* vs_file, const char* fs_file);
	virtual void BuildMaterial();
	virtual void Bind();
	virtual void Unbind();
	virtual MATERIAL_TYPE GetType()
	{
		return MATERIAL_TYPE_MONO;
	}
	void IncreaseReference()
	{
		m_ReferenceCount++;
	}
	void DecreaseReference()
	{
		m_ReferenceCount--;
	}
	virtual unsigned int GetTextureCount()
	{
		return 1;
	}
	bool IsUnused()
	{
		return m_ReferenceCount == 0;
	}
	GLuint GetTextureSizeX()
	{
		return m_TextureSize[0]; 
	}
	GLuint GetTextureSizeY()
	{
		return m_TextureSize[1]; 
	}
};