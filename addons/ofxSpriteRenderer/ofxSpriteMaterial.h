#pragma once
#include "ofMain.h"
struct ofxVertex
{
	float X,Y,Z;// position
	float UV[64];// texture uv
	float CUV[64];// crop uv, the value determine if we should use this pixel or not, cuv in (0,1) = true, else = false
};
class ofxSpriteMaterial
{
private:

private:
	unsigned int m_TextureCount;
private:
	unsigned int m_ReferenceCount;
private:
	GLuint* m_TextureId;
	GLuint* m_TextureOrder;
	GLuint* m_TextureSize;
	GLuint m_ShaderProgramId;
	GLint m_ShaderLocationXYZ;
	GLint m_ShaderLocationTextureCount;
	GLint* m_ShaderLocationUV;
	GLint* m_ShaderLocationCUV;
	GLint* m_ShaderLocationTexture;
public:
	ofxSpriteMaterial();
	~ofxSpriteMaterial();
	void SetMaxTexture(const int size);
	void LoadTexturePNG(const char* texture_file, const int index);
	bool LoadShader(const char* vs_file, const char* fs_file);
	void BuildMaterial();
	void Bind();
	void Unbind();
	void IncreaseReference()
	{
		m_ReferenceCount++;
	}
	void DecreaseReference()
	{
		m_ReferenceCount--;
	}
	bool IsUnused()
	{
		return m_ReferenceCount == 0;
	}
	unsigned int GetTextureCount()
	{
		return m_TextureCount;
	}
	GLuint GetTextureSizeRaw(const int index)
	{
		return m_TextureSize[index]; 
	}
	GLuint GetTextureSizeX(const int index)
	{
		return m_TextureSize[index*2]; 
	}
	GLuint GetTextureSizeY(const int index)
	{
		return m_TextureSize[index*2+1]; 
	}
	void SetOrder(const int texture_id, const int order)
	{
		m_TextureOrder[order] = texture_id;
	}
};