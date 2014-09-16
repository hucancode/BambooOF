#include "ofxParticleEffect2D.h"
#include "ofxTextureCache.h"
#include "ofxSpriteRenderer.h"

ofxParticleEffect2D::ofxParticleEffect2D()
{
	ofxSpriteBase::ofxSpriteBase();
	m_Vertices = new ofxVertex[MAX_PARTICLE2D_COUNT];
	m_ParticleCount = 0;
	m_Paused = false;
	m_Texture = m_SharedParticleTexture;
	LoadShader(DEFAULT_PARTICLE_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxParticleEffect2D::~ofxParticleEffect2D()
{
}
void ofxParticleEffect2D::Load(string xml_file)
{
}
void ofxParticleEffect2D::AddEmitter(ofxEmitter2D* emitter)
{
	m_Emitters.push_back(emitter);
}
void ofxParticleEffect2D::Update(float delta_time)
{
	if(m_Paused) return;
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
				ofxParticle2D& item = m_ParticlePool[j];
				ofxVertex& v0 = item.vertices[0];
				ofxVertex& v1 = item.vertices[1];
				ofxVertex& v2 = item.vertices[2];
				ofxVertex& v3 = item.vertices[3];
				// speed
				item.emitter = e;
				item.life = e->life + ofRandom(e->life_var);
				item.speed = e->speed + ofRandom(e->speed_var);
				item.radial_accel = e->radial_accel + ofRandom(e->radial_accel_var);
				item.tangental_accel = e->tangental_accel + ofRandom(e->tangental_accel_var);
				// position
				float radius = e->radius + ofRandom(e->radius_var);
				float angle = e->angle + ofRandom(e->angle_var);
				item.position = radius*ofVec2f(1,1).rotateRad(angle);
				item.size = e->begin_size;
				// uv
				ofRectangle texture_rect = m_SharedParticleUVs[(int)ofRandom(m_SharedParticleUVs.size()-1)];
				v0.u = texture_rect.getMinX();
				v0.v = texture_rect.getMinY();
				v1.u = texture_rect.getMaxX();
				v1.v = texture_rect.getMinY();
				v2.u = texture_rect.getMaxX();
				v2.v = texture_rect.getMaxY();
				v3.u = texture_rect.getMinX();
				v3.v = texture_rect.getMaxY();
				// TODO: color
				v0.r = 0; v0.g = 0; v0.b = 0; v0.color_intensity = 0; v0.opacity = 1.0f;
				v1.r = 0; v1.g = 0; v1.b = 0; v1.color_intensity = 0; v1.opacity = 1.0f;
				v2.r = 0; v2.g = 0; v2.b = 0; v2.color_intensity = 0; v2.opacity = 1.0f;
				v3.r = 0; v3.g = 0; v3.b = 0; v3.color_intensity = 0; v3.opacity = 1.0f;
			}
			e->cooldown = e->emission_time + ofRandom(e->emission_time_var);
		}
	}
	for(int i=0;i<m_ParticleCount;i++)
	{
		ofxParticle2D& item = m_ParticlePool[i];
		ofxVertex& v0 = item.vertices[0];
		ofxVertex& v1 = item.vertices[1];
		ofxVertex& v2 = item.vertices[2];
		ofxVertex& v3 = item.vertices[3];
		item.life -= delta_time;
		while(item.life <= 0 || item.vertices[0].opacity <= 0)// life
		{
			m_ParticleCount--;
			if(m_ParticleCount == 0) break;
			ofxParticle2D p = m_ParticlePool[m_ParticleCount];
			m_ParticlePool[m_ParticleCount] = m_ParticlePool[i];
			m_ParticlePool[i] = p;
			item = m_ParticlePool[i];
		}
		if(m_ParticleCount == 0) break;
		{// force
			ofVec2f radial_force, tangental_force;
#if defined READABILITY
			// this code isn't optimized for readability, 10 calculation
			float radial = item.emitter->position.angleRad(item.position);
			float tangental = radial + HALF_PI;
			radial_force = delta_time*item.radial_accel*item.speed*ofVec2f(1,1).rotateRad(radial);
			tangental_force = delta_time*item.tangental_accel*item.speed*ofVec2f(1,1).rotateRad(tangental);
#else
			// this code has same function as above, 7 calculation
			float radial = item.emitter->position.angleRad(item.position);
			ofVec2f radial_force_normalized = delta_time*item.speed*ofVec2f(1,1).rotateRad(radial);
			ofVec2f tangental_force_normalized = radial_force_normalized.rotateRad(HALF_PI);
			radial_force = item.radial_accel*radial_force_normalized;
			tangental_force = item.tangental_accel*tangental_force_normalized;
#endif
			item.position += radial_force;
			item.position += tangental_force;
			float half_size = item.size*0.5f;
			v0.x = item.position.x - half_size;
			v0.y = item.position.y - half_size;
			v0.z = m_Position.z;
			v1.x = v0.x + item.size;
			v1.y = v0.y;
			v1.z = v0.z;
			v2.x = v1.x;
			v2.y = v1.y + item.size;
			v2.z = v1.z;
			v3.x = v0.x;
			v3.y = v2.y;
			v3.z = v2.z;
		}
		{// TODO: color
			v0.r = 0; v0.g = 0; v0.b = 0; v0.color_intensity = 0; v0.opacity = 1.0f;
			v1.r = 0; v1.g = 0; v1.b = 0; v1.color_intensity = 0; v1.opacity = 1.0f;
			v2.r = 0; v2.g = 0; v2.b = 0; v2.color_intensity = 0; v2.opacity = 1.0f;
			v3.r = 0; v3.g = 0; v3.b = 0; v3.color_intensity = 0; v3.opacity = 1.0f;
		}
	}
}
void ofxParticleEffect2D::SubmitChanges()
{
	m_VerticesSize = m_ParticleCount*4;
	int size = sizeof(ofxVertex)*4;
	for(int i=0;i<m_ParticleCount;i++)
	{
		memcpy(m_Vertices+i*4, m_ParticlePool[i].vertices, size);
	}
}
void ofxParticleEffect2D::PauseResume()
{
	m_Paused = !m_Paused;
}
bool ofxParticleEffect2D::IsPaused()
{
	return m_Paused;
}
vector<ofRectangle>	ofxParticleEffect2D::m_SharedParticleUVs;
ofxTexture* ofxParticleEffect2D::m_SharedParticleTexture = 0;
void ofxParticleEffect2D::LoadSharedParticleTexture()
{
	m_SharedParticleTexture = ofxTEXTURECACHE->GetResource("data/particle_sheet.png");
	m_SharedParticleUVs.push_back(ofRectangle(0,0,1,1));
}