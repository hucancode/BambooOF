#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
class ofxSpriteCommand;
class ofxSpriteQuad
{
	friend ofxSpriteCommand;
private:
	ofxSpriteMaterial* m_Material;
	ofVec3f m_WorldPosition;
	GLfloat m_WorldQuad[2];
	unsigned int m_Quad[2];
	unsigned int* m_TextureRect;
	unsigned int* m_SpriteRect;
	unsigned int m_IndexInCommand;
	ofxSpriteCommand* m_ParentCommand;
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	ofxSpriteMaterial* GetMaterial()
	{
		return m_Material;
	}
	void SetMaterial(ofxSpriteMaterial* material)
	{
		m_Material = material;
	}
	ofVec3f GetPosition()
	{
		return m_WorldPosition;
	}
	void SetPosition(const ofVec3f position)
	{
		m_WorldPosition = position;
	}
	void SetPosition(const float x, const float y, const float z)
	{
		SetPosition(ofVec3f(x,y,z));
	}
	unsigned int GetWidth()
	{
		return m_Quad[0];
	}
	void SetWidth(const unsigned int width)
	{
		m_Quad[0] = width;
	}
	unsigned int GetHeight()
	{
		return m_Quad[1];
	}
	void SetHeight(const unsigned int height)
	{
		m_Quad[1] = height;
	}
	unsigned int GetTextureRectRaw(const int index)
	{
		return m_TextureRect[index];
	}
	unsigned int GetTextureRectX(const int index)
	{
		return m_TextureRect[index*4];
	}
	unsigned int GetTextureRectY(const int index)
	{
		return m_TextureRect[index*2+1];
	}
	unsigned int GetTextureRectW(const int index)
	{
		return m_TextureRect[index*4+2];
	}
	unsigned int GetTextureRectH(const int index)
	{
		return m_TextureRect[index*4+3];
	}
	unsigned int GetSpriteRectRaw(const int index)
	{
		return m_SpriteRect[index];
	}
	unsigned int GetSpriteRectX(const int index)
	{
		return m_SpriteRect[index*4];
	}
	unsigned int GetSpriteRectY(const int index)
	{
		return m_SpriteRect[index*2+1];
	}
	unsigned int GetSpriteRectW(const int index)
	{
		return m_SpriteRect[index*4+2];
	}
	unsigned int GetSpriteRectH(const int index)
	{
		return m_SpriteRect[index*4+3];
	}
	void SetMaxTexture(const int size);
	void SetTextureRect(const unsigned int x, const unsigned int y, 
		const unsigned int w, const unsigned int h, const int order);
	void SetSpriteRect(const unsigned int x, const unsigned int y, 
		const unsigned int w, const unsigned int h, const unsigned int order);
};
#define ofxSpriteQuads vector<ofxSpriteQuad*>