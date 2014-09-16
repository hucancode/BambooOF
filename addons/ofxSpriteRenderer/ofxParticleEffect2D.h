#pragma once
#include "ofxSpriteBase.h"

/*
note: this effect has all of it's particle stick to a plane
*/

const int MAX_PARTICLE2D_COUNT = 200;

struct ofxEmitter2D
{
	ofVec2f		position;
	int			emission_rate;
	int			emission_rate_var;
	float		emission_time;
	float		emission_time_var;
	float		cooldown;
	float		life;
	float		life_var;
	float		radius;
	float		radius_var;
	float		speed;
	float		speed_var;
	float		angle;
	float		angle_var;
	float		radial_accel;
	float		radial_accel_var;
	float		tangental_accel;
	float		tangental_accel_var;
	float		begin_size;
	float		begin_opacity;
	float		begin_opacity_var;
	float		begin_color_intensity;
	float		begin_color_intensity_var;
	ofColor		begin_color;
	ofColor		begin_color_var;
	float		end_size;
	float		end_opacity;
	float		end_opacity_var;
	float		end_color_intensity;
	float		end_color_intensity_var;
	ofColor		end_color;
	ofColor		end_color_var;
};

struct ofxParticle2D
{
	ofxEmitter2D*	emitter;
	ofVec2f			position;
	float			size;
	float			life;
	float			speed;
	float			radial_accel;
	float			tangental_accel;
	ofxVertex		vertices[4];
};

typedef vector<ofxEmitter2D*> ofxEmitter2Ds;
class ofxParticleEffect2D
	:public ofxSpriteBase
{
private:
	static ofxTexture*			m_SharedParticleTexture;
	static vector<ofRectangle>	m_SharedParticleUVs;
private:
	ofxEmitter2Ds				m_Emitters;
	ofxParticle2D				m_ParticlePool[MAX_PARTICLE2D_COUNT];
	unsigned short				m_ParticleCount;
	bool						m_Paused;
public:
	static void					LoadSharedParticleTexture();
public:
	ofxParticleEffect2D();
	~ofxParticleEffect2D();
	void						Load(string xml_file);
	void						AddEmitter(ofxEmitter2D* emitter);
	virtual void				Update(float delta_time);
	virtual void				SubmitChanges();
	void						PauseResume();
	bool						IsPaused();
};
#define DEFAULT_PARTICLE_SHADER "sprite2d"