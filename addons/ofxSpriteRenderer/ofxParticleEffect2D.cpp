#include "ofxParticleEffect2D.h"
ofxParticleEffect2D::ofxParticleEffect2D()
{
}
ofxParticleEffect2D::~ofxParticleEffect2D()
{
}
void ofxParticleEffect2D::Load(string xml_file)
{
}
void ofxParticleEffect2D::AddEmitter(ofxEmitter2D* emitter)
{
}
void ofxParticleEffect2D::Update(float delta_time)
{
	for(int i = 0;i < m_Emitters.size();i++)
	{
		ofxEmitter2D* e = m_Emitters[i];
		e->cooldown -= delta_time;
		if(e->cooldown <= 0)
		{
			int particle_to_emit = e->emission_rate + ofRandom(e->emission_rate_var);
			int j = m_ParticleCount;
			m_ParticleCount += particle_to_emit;
			for(;j<m_ParticleCount;j++)
			{
				m_ParticlePool[j].emitter = e;
				m_ParticlePool[j].life = e->life + ofRandom(e->life_var);
				m_ParticlePool[j].speed = e->speed + ofRandom(e->speed_var);
				m_ParticlePool[j].radial_accel = e->radial_accel + ofRandom(e->radial_accel_var);
				m_ParticlePool[j].tangental_accel = e->tangental_accel + ofRandom(e->tangental_accel_var);
				float radius = e->radius + ofRandom(e->radius_var);
				float angle = e->angle + ofRandom(e->angle_var);
				m_ParticlePool[j].position = radius*ofVec2f(1,1).rotateRad(angle);
				// TODO: calculate these, you know, color
				m_ParticlePool[j].vertices[0];
				m_ParticlePool[j].vertices[1];
				m_ParticlePool[j].vertices[2];
				m_ParticlePool[j].vertices[3];
			}
			e->cooldown = e->emission_time + ofRandom(e->emission_time_var);
		}
	}
	for(int i=0;i<m_ParticleCount;i++)
	{
		ofxParticle2D& item = m_ParticlePool[i];
		item.life -= delta_time;
		if(item.life <= 0 || item.vertices[0].opacity <= 0)// life
		{
			// TODO: do kill particle
		}
		{// force
			ofVec2f radial_force, tangental_force;
#if defined READABILITY
			// this code isn't optimized for readability, 8 calculation
			float radial = item.emitter->position.angleRad(item.position);
			float tangental = radial + HALF_PI;
			radial_force = item.radial_accel*item.speed*ofVec2f(1,1).rotateRad(radial);
			tangental_force = item.tangental_accel*item.speed*ofVec2f(1,1).rotateRad(tangental);
#else
			// this code has same function as above, 6 calculation
			float radial = item.emitter->position.angleRad(item.position);
			ofVec2f radial_force_normalized = item.speed*ofVec2f(1,1).rotateRad(radial);
			ofVec2f tangental_force_normalized = radial_force_normalized.rotateRad(HALF_PI);
			radial_force = item.radial_accel*radial_force_normalized;
			tangental_force = item.tangental_accel*tangental_force_normalized;
#endif
			item.position += radial_force;
			item.position += tangental_force;
		}
		{// TODO: appearance

		}
	}
}
void ofxParticleEffect2D::SubmitChanges()
{
}
void ofxParticleEffect2D::PauseResume()
{
}
bool ofxParticleEffect2D::IsPaused()
{
	return true;
}