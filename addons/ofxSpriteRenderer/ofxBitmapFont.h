#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxTexture.h"
#include "IL/il.h"
#include "IL/ilu.h"

class ofxBitmapFont
	: public ofxResource
{
private:
	map<char, ofVec4f>		m_CharacterMap;
	map<char, ILuint>		m_BitmapCache;
	unsigned char			m_FontSize;
public:
	ofxBitmapFont();
	virtual ~ofxBitmapFont();
	virtual bool			Load(string xml_file);
	unsigned char			GetFontSize();
public:
	ofVec4f					GetRect(char character);
	ofVec2f					GetTextDimension(string text, unsigned char font_size = 0);
	ILuint					GetImageId(char character);
};
typedef vector<ofxBitmapFont*> ofxBitmapFonts;