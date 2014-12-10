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
	GLuint				m_VBOId;
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
	JX_ANIMATION_STATE	m_CurrentState;
	JX_GENDER			m_Gender;
	ofxTextures			m_Textures;
	JxVertices			m_JxVertices;
	float				m_FrameTimer;
	float				m_FrameTime;
	unsigned char		m_FrameMax;
	unsigned char		m_FrameMin;
	unsigned char		m_CurrentFrame;
public:
	JxAnimationCombo();
	~JxAnimationCombo();
	void				SetGender				(const JX_GENDER gender);
	void				SetAction				(const JX_ACTION_STATE action_state);
	void				SetWeapon				(const JX_WEAPON_STATE weapon_state);
	void				SetHorse				(const JX_HORSE_STATE horse_state);
public:
	void				SetColor				(const ofFloatColor color);
	void				SetColorIntensity		(const float color_intensity);
	void				SetOpacity				(const float opacity);
public:
	void				SetHelmAnimation		(const string name);
	void				SetClothAnimation		(const string name);
	void				SetHandLAnimation		(const string name);
	void				SetHandRAnimation		(const string name);
	void				SetWeaponLightAnimation	(const string name);
	void				SetWeaponHeavyAnimation	(const string name);
	void				SetWeaponDualLAnimation	(const string name);
	void				SetWeaponDualRAnimation	(const string name);
	void				SetHorseHeadAnimation	(const string name);
	void				SetHorseBackAnimation	(const string name);
	void				SetHorseTailAnimation	(const string name);
private:
	void				LoadAnimation			(JxAnimation* animation, string path, const string name);
public:
	virtual void		Update					(const float delta_time);
	virtual void		SubmitChanges			();
	virtual void		Render					();
};
#define DEFAULT_JXANIMATION_SHADER "jx_animation"