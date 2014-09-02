#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"
#include "ofxShaderProgram.h"
#include "ofxBitmapFont.h"

class ofxSpriteText
	:public ofxSpriteBase
{
private:
	string				m_Text;
	vector<ofxVertex>	m_Vertices;
	ofxBitmapFont*		m_Font;
	ofColor				m_BackgroundColor;
	float				m_Scale;
	bool				m_AutoEndLine;
private:
	ofxSpriteText();
	~ofxSpriteText();
	void				SetFont(string font_name);
	ofxBitmapFont*		GetFont();
	void				SetText(string text);
	string				GetText();
	void				SetBackgroundColor(ofColor color);
	ofColor				GetBackgroundColor();
	void				SetScale(float scale);
	float				GetScale();
	void				SetAutoEndLine(bool value);
	bool				IsAutoEndLine();
	void				BuildMesh();
	virtual void		SubmitChanges();
};
typedef vector<ofxSpriteText*> ofxSpriteTexts;