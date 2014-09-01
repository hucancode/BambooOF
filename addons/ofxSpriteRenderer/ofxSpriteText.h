#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"
#include "ofxShaderProgram.h"


struct ofxFont
{

};
class ofxSpriteText
{
private:
	string				m_Text;
	vector<ofxVertex>	m_Vertices;	
private:
	ofxSpriteText();
	~ofxSpriteText();
	void				SetText(string text);
	void				BuildMesh();
	virtual void		SubmitChanges();
};