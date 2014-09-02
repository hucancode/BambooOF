#include "ofxBitmapFont.h"
ofxBitmapFont::ofxBitmapFont()
	:ofxResource()
{
	m_Texture = 0;
}
ofxBitmapFont::~ofxBitmapFont()
{
}
bool ofxBitmapFont::Load(string xml_file)
{
	return true;
}
void ofxBitmapFont::IncreaseReference()
{
	ofxResource::IncreaseReference();
}
void ofxBitmapFont::DecreaseReference()
{
	ofxResource::DecreaseReference();
}
bool ofxBitmapFont::IsUnused()
{
	return ofxResource::IsUnused();
}
ofxTexture* ofxBitmapFont::GetTexture()
{
	return m_Texture; 
}
ofVec4f ofxBitmapFont::GetCharacterRect(char character)
{
	return ofVec4f::zero();
}