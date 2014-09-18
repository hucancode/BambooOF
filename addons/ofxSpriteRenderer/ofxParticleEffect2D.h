#pragma once
#include "ofxSpriteBase.h"

/*
note: this effect has all of it's particle stick to a plane
*/

// TODO: if i have time, i would like to implement some affector, such as gravity, wind, magnet ....

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
	float		accel;
	float		accel_var;
	float		angle;
	float		angle_var;
	float		radial_accel;
	float		radial_accel_var;
	float		tangental_accel;
	float		tangental_accel_var;
	float		size;
	float		size_var;
	float		size_accel;
	float		opacity;
	float		opacity_var;
	float		opacity_accel;
	float		color_intensity;
	float		color_intensity_var;
	ofColor		color;
	ofColor		color_var;
	ofColor		color_accel;

};

struct ofxParticle2D
{
	ofxEmitter2D*	emitter;
	ofVec2f			position;
	float			size;
	float			life;
	float			speed;
	float			accel;
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
	static const float			DEGREE_TO_RADIAL;
	static const float			RADIAL_TO_DEGREE;
	static float				m_SinTable[360];
	static float				m_CosTable[360];
private:
	ofxEmitter2Ds				m_Emitters;
	ofxParticle2D				m_ParticlePool[MAX_PARTICLE2D_COUNT];
	unsigned short				m_ParticleCount;
	bool						m_Paused;
	bool						m_Stopped;
public:
	static void					LoadSharedParticleTexture();
	static void					BuildSinCosTable();
	static ofVec2f				GetForceFromAngle(int angle);
	static float				GetAngleFromForceRadial(ofVec2f force);
	static int					GetAngleFromForce(ofVec2f force);
public:
	ofxParticleEffect2D();
	~ofxParticleEffect2D();
	void						Load(string xml_file);
	void						AddEmitter(ofxEmitter2D* emitter);
	virtual void				Update(float delta_time);
	virtual void				SubmitChanges();
	void						PauseResume();
	bool						IsPaused();
	void						Stop();
	void						ForceStop();
	bool						IsStopped();
};
#define DEFAULT_PARTICLE_SHADER "particle2d"