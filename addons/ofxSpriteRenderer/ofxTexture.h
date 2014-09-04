#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "FreeImage.h"

class ofxBitmapFont;

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
public:
	void				Allocate(unsigned int width, unsigned int height);
	ofColor				GetPixel(unsigned int x, unsigned int y);
	void				SetPixel(unsigned int x, unsigned int y, ofColor color);
	void				BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos);
	void				StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect);
	void				Fill(ofColor color, ofRectangle dest_rect);
	void				Clear(ofRectangle dest_rect);
	void				DrawString(ofxBitmapFont* font, string text, ofVec2f dest_pos, 
									float font_size = 10.0f, ofVec2f bound = ofVec2f::zero());
};
typedef vector<ofxTexture*> ofxTextures;