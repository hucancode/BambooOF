#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "FreeImage.h"
class ofxTexture
	: public ofxResource
{
public:
	static ofxTexture* s_ActivatedTexture[16];
private:
	FIBITMAP* m_ImageData;
private:
	GLuint m_TextureId;
	ofVec2f m_TextureSize;
public:
	ofxTexture();
	virtual ~ofxTexture();
	virtual bool Load(string texture_file);
	virtual void IncreaseReference();
	virtual void DecreaseReference();
	virtual bool IsUnused();
	void Bind(GLuint slot=0);
	void Unbind();
	ofVec2f GetTextureSize()
	{
		return m_TextureSize; 
	}
};
typedef vector<ofxTexture*> ofxTextures;