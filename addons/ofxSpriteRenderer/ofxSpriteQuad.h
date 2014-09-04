#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"


class ofxSpriteCommand;
class ofxSpriteRenderer;
class ofxSpriteQuad : public ofxSpriteBase
{
	friend ofxSpriteCommand;
	friend ofxSpriteRenderer;
protected:
	bool				m_UVChange;
	ofRectangle			m_TextureRect;
	ofRectangle			m_SpriteRect;
protected:
	float				m_ScaleX;
	float				m_ScaleY;
	bool				m_MirrorX;
	bool				m_MirrorY;
	unsigned char		m_Opacity;
	ofColor				m_Color;
	unsigned char		m_ColorIntensity;
public:
	ofxSpriteQuad();
	~ofxSpriteQuad();
	virtual void		Update(const float delta_time){}
	virtual void		SetTexture(string texture_path);
	void				SetTextureRect(const float x, const float y, const float w, const float h);
	void				SetTextureRect(const ofRectangle rect);
	void				SetSpriteRect(const float x, const float y, const float w, const float h);
	void				SetSpriteRect(const ofRectangle rect);
	ofRectangle			GetTextureRect();
	ofRectangle			GetSpriteRect();
	virtual void		SubmitChanges();
public:
	void				SetScale(float value);
	void				SetScaleX(float value);
	float				GetScaleX();
	void				SetScaleY(float value);
	float				GetScaleY();
	void				SetMirrorX(bool value);
	bool				IsMirrorX();
	void				SetMirrorY(bool value);
	bool				IsMirrorY();
	void				SetOpacity(unsigned char value);
	unsigned char		GetOpacity();
	void				SetColor(ofColor value);
	ofColor				GetColor();
	void				SetColorIntensity(unsigned char value);
	unsigned char		GetColorIntensity();
private:
	void				SetUVChange();
	bool				GetUVChange();
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;
#define DEFAULT_SHADER "sprite2d"