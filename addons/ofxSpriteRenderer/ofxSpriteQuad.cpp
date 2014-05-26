#include "ofxSpriteQuad.h"
ofxSpriteQuad::ofxSpriteQuad()
{
	m_TextureRect = 0;
	m_Material = 0;
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	if(m_TextureRect) delete[] m_TextureRect;
}
void ofxSpriteQuad::SetMaxTexture(const int size)
{
	if(m_TextureRect) delete[] m_TextureRect;
	m_TextureRect = new float[size*4];
	if(m_SpriteRect) delete[] m_SpriteRect;
	m_SpriteRect = new float[size*4];
}
void ofxSpriteQuad::SetTextureRect(const unsigned int x, const unsigned int y, 
								   const unsigned int w, const unsigned int h, const unsigned int order)
{
	int order4 = order*4;
	m_TextureRect[order4] = x;
	m_TextureRect[++order4] = y;
	m_TextureRect[++order4] = w;
	m_TextureRect[++order4] = h;
}
void ofxSpriteQuad::SetSpriteRect(const unsigned int x, const unsigned int y, 
								  const unsigned int w, const unsigned int h, const unsigned int order)
{
	int order4 = order*4;
	m_SpriteRect[order4] = x;
	m_SpriteRect[++order4] = y;
	m_SpriteRect[++order4] = w;
	m_SpriteRect[++order4] = h;
}