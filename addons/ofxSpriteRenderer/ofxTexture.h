#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "FreeImage.h"
class ofxTexture
	: public ofxResource
{
private:
	FIBITMAP*			m_ImageData;
private:
	GLuint				m_TextureId;
	ofVec2f				m_TextureSize;
public:
	ofxTexture();
	virtual ~ofxTexture();
	virtual bool		Load(string texture_file);
	virtual void		IncreaseReference();
	virtual void		DecreaseReference();
	virtual bool		IsUnused();
	void				Bind(GLuint slot=0);
	void				Unbind(GLuint slot=0);
	ofVec2f				GetTextureSize();
};
typedef vector<ofxTexture*> ofxTextures;