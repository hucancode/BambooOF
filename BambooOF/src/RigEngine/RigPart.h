#pragma once
#include "ofMain.h"
#include "ofxBaseSprite.h"
#include "ofxBaseCommand.h"
class RigPart
{
	friend class RigPartRender;
private:
	string				m_Name;
	unsigned char		m_Part;
	unsigned char		m_Action;
	bool				m_Gender;
	unsigned char		m_State;
	bool				m_UVChange;
	bool				m_TextureChange;
public:
	RigPart();
	~RigPart();
	void				Update(float delta_time);
	void				SetAction(unsigned char action);
	void				SetPart(unsigned char part);
	void				SetGender(bool gender);
	void				SetState(unsigned char state);
	void				SetName(string name);
};

class RigPartRenderer
	:public ofxBaseSprite, public ofxBaseCommand
{
private:
	RigPart*			m_RigPartObject;
public:
	RigPartRenderer();
	~RigPartRenderer();
	void				SetObject(RigPart* object);
	void				Update(float delta_time);
	void				ApplyChanges();
	virtual void		Render();
};