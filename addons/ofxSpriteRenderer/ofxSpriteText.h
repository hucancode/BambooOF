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
	bool				m_AutoWidthHeight;
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
	void				SetDimension(ofRectangle rect);
	ofRectangle			GetDimension();
	void				SetAutoEndLine(bool value);
	bool				IsAutoEndLine();
	void				SetAutoWidthHeight(bool value);
	bool				IsAutoWidthHeight();
	void				BuildMesh();
	virtual void		SubmitChanges();
};
typedef vector<ofxSpriteText*> ofxSpriteTexts;