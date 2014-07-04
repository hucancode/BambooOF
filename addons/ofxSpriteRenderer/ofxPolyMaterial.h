#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
#include "FreeImage.h"
class ofxPolyMaterial
	:public ofxSpriteMaterial
{
private:
	unsigned int m_TextureCount;
private:
	unsigned int m_ReferenceCount;
private:
	FIBITMAP** m_ImageData;
private:
	GLuint* m_TextureId;
	GLuint* m_TextureOrder;
	ofVec2f* m_TextureSize;
	GLuint m_ShaderProgramId;
	GLint m_ShaderLocationXYZ;
	GLint m_ShaderLocationTextureCount;
	GLint* m_ShaderLocationUV;
	GLint* m_ShaderLocationCUV;
	GLint* m_ShaderLocationTexture;
	GLint m_ShaderLocationTransform;
	GLint m_ShaderLocationInvModelView;
public:
	ofxPolyMaterial();
	~ofxPolyMaterial();
	void SetMaxTexture(const int size);
	void LoadTexturePNG(const char* texture_file, const int index);
	virtual bool LoadShader(const char* vs_file, const char* fs_file);
	virtual void BuildMaterial();
	virtual void Bind();
	virtual void Unbind();
	virtual MATERIAL_TYPE GetType()
	{
		return MATERIAL_TYPE_POLY;
	}
	virtual void IncreaseReference()
	{
		m_ReferenceCount++;
	}
	virtual void DecreaseReference()
	{
		m_ReferenceCount--;
		if(m_ReferenceCount == 0)
		{
			delete this;
		}
	}
	virtual unsigned int GetTextureCount()
	{
		return m_TextureCount;
	}
	virtual ofVec2f GetTextureSize(const int index)
	{
		return m_TextureSize[index]; 
	}
	void SetOrder(const int texture_id, const int order)
	{
		m_TextureOrder[order] = texture_id;
	}
};