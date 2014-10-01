#pragma once
#include "RigPart.h"
#include "ofxBaseSprite.h"
#include "ofxBaseCommand.h"
#include "StateCollectionJX.h"

/* ----------------------------
// WIP
---------------------------- */

struct RigVertex
{
	float x, y, z;
	float u, v;
	int texture;
};
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
	void				SetHelm					(RigPart* object);
	void				SetCloth				(RigPart* object);
	void				SetHandL				(RigPart* object);
	void				SetHandR				(RigPart* object);
	void				SetWeaponO				(RigPart* object);
	void				SetWeaponT				(RigPart* object);
	void				SetWeaponDL				(RigPart* object);
	void				SetWeaponDR				(RigPart* object);
	void				SetHorseHead			(RigPart* object);
	void				SetHorseBack			(RigPart* object);
	void				SetHorseTail			(RigPart* object);
	virtual void		Update					(const float delta_time);
	void				ApplyChanges			();
	virtual void		Render					();
private:
	static string		getTexturePath			(RigPart part);
};