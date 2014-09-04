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
	ofColor				GetPixel(ofVec2f position);
	void				SetPixel(ofVec2f position, ofColor color);
	void				FlipX();
	void				FlipY();
	void				BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos, int alpha = 255);
	void				StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect, int alpha = 255);
	void				Fill(ofColor color, ofRectangle dest_rect);
	void				Clear(ofRectangle dest_rect);
	void				Clear();
	void				DrawString(ofxBitmapFont* font, string text, ofVec2f dest_pos, 
									unsigned char font_size = 10, ofVec2f bound = ofVec2f::zero());
};
typedef vector<ofxTexture*> ofxTextures;