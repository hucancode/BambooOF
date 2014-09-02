#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxTexture.h"
#include "ofxXmlSettings.h"
class ofxBitmapFont
	: public ofxResource
{
private:
	ofxTexture*			m_Texture;
	map<char, ofVec4f>	m_CharacterMap;
public:
	ofxBitmapFont();
	virtual ~ofxBitmapFont();
	virtual bool		Load(string xml_file);
	virtual void		IncreaseReference();
	virtual void		DecreaseReference();
	virtual bool		IsUnused();
	ofxTexture*			GetTexture();
	ofVec4f				GetCharacterRect(char character);
};
typedef vector<ofxBitmapFont*> ofxBitmapFonts;