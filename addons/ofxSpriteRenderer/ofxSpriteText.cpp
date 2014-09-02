#include "ofxSpriteText.h"
ofxSpriteText::ofxSpriteText()
{

}
ofxSpriteText::~ofxSpriteText()
{
}

void ofxSpriteText::BuildMesh()
{
}
void ofxSpriteText::SubmitChanges()
{
}
void ofxSpriteText::SetFont(string font_name)
{
	//....
}
ofxBitmapFont* ofxSpriteText::GetFont()
{
	return m_Font;
}
void ofxSpriteText::SetText(string text)
{
	m_Text = text;
}
string ofxSpriteText::GetText()
{
	return m_Text;
}
void ofxSpriteText::SetBackgroundColor(ofColor color)
{
	m_BackgroundColor = color;
}
ofColor ofxSpriteText::GetBackgroundColor()
{
	return m_BackgroundColor;
}
void ofxSpriteText::SetScale(float scale)
{
	m_Scale = scale;
}
float ofxSpriteText::GetScale()
{
	return m_Scale;
}
void ofxSpriteText::SetAutoEndLine(bool value)
{
	m_AutoEndLine = value;
}
bool ofxSpriteText::IsAutoEndLine()
{
	return m_AutoEndLine;
}