#pragma once
#include "RigPart.h"
#include "ofxBaseSprite.h"
#include "ofxBaseCommand.h"
#include "StateCollectionJX.h"

/* ----------------------------
// WIP
---------------------------- */

class RigCollectionJX
	:public ofxBaseSprite, public ofxBaseCommand
{
private:
	RigPart*			m_Helm;
	RigPart*			m_Cloth;
	RigPart*			m_HandL;
	RigPart*			m_HandR;
	RigPart*			m_WeaponO;
	RigPart*			m_WeaponT;
	RigPart*			m_WeaponDL;
	RigPart*			m_WeaponDR;
	RigPart*			m_HorseHead;
	RigPart*			m_HorseBack;
	RigPart*			m_HorseTail;

	ofFloatColor		m_Color;
	float				m_ColorIntensity;
	float				m_Opacity;
	ofxTextures			m_Textures;
	RigVertex*			m_RigVertices;

	float				m_FrameTime;
	unsigned char		m_FrameCount;
	unsigned char		m_CurrentFrame;
public:
	RigCollectionJX();
	~RigCollectionJX();
	void				SetGender				(bool gender);
	void				SetState				(RIG_ENGINE_STATE state);
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
	static string		GetTexturePath			(RigPart* part);
};