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
	JxAnimation*		m_Helm;
	JxAnimation*		m_Cloth;
	JxAnimation*		m_HandL;
	JxAnimation*		m_HandR;
	JxAnimation*		m_WeaponLight;
	JxAnimation*		m_WeaponHeavy;
	JxAnimation*		m_WeaponDualL;
	JxAnimation*		m_WeaponDualR;
	JxAnimation*		m_HorseHead;
	JxAnimation*		m_HorseBack;
	JxAnimation*		m_HorseTail;
	JxAnimations		m_Effects;

	ofFloatColor		m_Color;
	float				m_ColorIntensity;
	float				m_Opacity;
	ofxTextures			m_Textures;
	JxVertices			m_JxVertices;
	float				m_FrameTimer;
	float				m_FrameTime;
	unsigned char		m_FrameMax;
	unsigned char		m_FrameMin;
	unsigned char		m_CurrentFrame;
	JX_ANIMATION_STATE	m_CurrentState;
public:
	JxAnimationCombo();
	~JxAnimationCombo();
	void				SetGender				(const bool gender);
	void				SetState				(const JX_ANIMATION_STATE state);
	void				SetColor				(const ofFloatColor color);
	void				SetColorIntensity		(const float color_intensity);
	void				SetOpacity				(const float opacity);
	void				SetHelm					(const string name);
	void				SetCloth				(const string name);
	void				SetHandL				(const string name);
	void				SetHandR				(const string name);
	void				SetWeaponFree			();
	void				SetWeaponLight			(const string name);
	void				SetWeaponHeavy			(const string name);
	void				SetWeaponDualL			(const string name);
	void				SetWeaponDualR			(const string name);
	void				SetHorseHead			(const string name);
	void				SetHorseBack			(const string name);
	void				SetHorseTail			(const string name);
	virtual void		Update					(const float delta_time);
	virtual void		SubmitChanges			();
	virtual void		Render					();
};