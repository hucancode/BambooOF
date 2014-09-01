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
private:
	void				SetUVChange();
	bool				GetUVChange();
};
typedef vector<ofxSpriteQuad*> ofxSpriteQuads;
#define DEFAULT_SHADER "sprite2d"