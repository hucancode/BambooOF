#pragma once
#include "JxAnimation.h"
#include "ofxBaseSprite.h"
#include "ofxBaseCommand.h"
#include "JxAnimationState.h"

/* ----------------------------
// WIP
---------------------------- */

class JxAnimationCombo
	:public ofxBaseSprite, public ofxBaseCommand
{
private:
	JxAnimation*			m_Helm;
	JxAnimation*			m_Cloth;
	JxAnimation*			m_HandL;
	JxAnimation*			m_HandR;
	JxAnimation*			m_WeaponO;
	JxAnimation*			m_WeaponT;
	JxAnimation*			m_WeaponDL;
	JxAnimation*			m_WeaponDR;
	JxAnimation*			m_HorseHead;
	JxAnimation*			m_HorseBack;
	JxAnimation*			m_HorseTail;

	ofFloatColor		m_Color;
	float				m_ColorIntensity;
	float				m_Opacity;
	ofxTextures			m_Textures;
	JxVertex*			m_RigVertices;

	float				m_FrameTime;
	unsigned char		m_FrameCount;
	unsigned char		m_CurrentFrame;
public:
	JxAnimationCombo();
	~JxAnimationCombo();
	void				SetGender				(bool gender);
	void				SetState				(JX_ANIMATION_STATE state);
	void				SetColor				(ofFloatColor color);
	void				SetColorIntensity		(float color_intensity);
	void				SetOpacity				(float opacity);
	void				SetHelm					(string name);
	void				SetCloth				(string name);
	void				SetHandL				(string name);
	void				SetHandR				(string name);
	void				SetWeaponO				(string name);
	void				SetWeaponT				(string name);
	void				SetWeaponDL				(string name);
	void				SetWeaponDR				(string name);
	void				SetHorseHead			(string name);
	void				SetHorseBack			(string name);
	void				SetHorseTail			(string name);
	virtual void		Update					(const float delta_time);
	virtual void		SubmitChanges			();
	virtual void		Render					();
private:
	static string		GetTexturePath			(JxAnimation* part);
};