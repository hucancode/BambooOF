#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxTexture.h"

class ofxBitmapFont
	: public ofxResource
{
private:
	ofxTexture*			m_Texture;
	map<char, ofVec4f>	m_CharacterMap;
	unsigned char		m_FontSize;
public:
	ofxBitmapFont();
	virtual ~ofxBitmapFont();
	virtual bool		Load(string xml_file);
	virtual void		IncreaseReference();
	virtual void		DecreaseReference();
	virtual bool		IsUnused();
	ofxTexture*			GetTexture();
	ofVec4f				GetCharacterRect(char character);
	unsigned char		GetFontSize();
};
typedef vector<ofxBitmapFont*> ofxBitmapFonts;