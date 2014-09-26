#include "ofxParticleEffect3D.h"
#include "ofxTextureCache.h"
#include "ofxSpriteRenderer.h"
#include "math.h"

ofxParticleEffect3D::ofxParticleEffect3D()
{
	ofxBaseSprite::ofxBaseSprite();
	m_Vertices = new ofxVertex[MAX_PARTICLE3D_COUNT*4];
	m_ParticleCount = 0;
	m_Paused = false;
	m_Stopped = false;
	m_Texture = m_SharedParticleTexture;
	LoadShader(DEFAULT_PARTICLE_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxParticleEffect3D::~ofxParticleEffect3D()
{
	delete[] m_Vertices;
}
void ofxParticleEffect3D::Load(string xml_file)
{
}
void ofxParticleEffect3D::AddEmitter(ofxEmitter3D* emitter)
{
	m_Emitters.push_back(emitter);
}
void ofxParticleEffect3D::Update(float delta_time)
{
	if(m_Paused) return;
	for(int i = 0;i < m_Emitters.size();i++)
	{
		ofxEmitter3D* e = m_Emitters[i];
		e->cooldown -= delta_time;
		if(e->cooldown <= 0)
		{
			int particle_to_emit = e->emission_rate + ofRandom(e->emission_rate_var);
			int j = m_ParticleCount;
			m_ParticleCount += particle_to_emit;
			if(m_ParticleCount > MAX_PARTICLE3D_COUNT)
			{
				m_ParticleCount = MAX_PARTICLE3D_COUNT;
			}
			ofVec2f offset = m_Position + e->position;
			//ofLogNotice() << "emitting "<<m_ParticleCount<<endl;
			for(;j<m_ParticleCount;j++)
			{
				ofxParticle3D& item = m_ParticlePool[j];
				// speed
				item.emitter = e;
				item.life = e->life + ofRandom(e->life_var);
				item.speed = e->speed + ofRandom(e->speed_var);
				item.accel = e->accel + ofRandom(e->accel_var);
				item.radial_accel = e->radial_accel + ofRandom(e->radial_accel_var);
				item.tangental_x_accel = e->tangental_x_accel + ofRandom(e->tangental_x_accel_var);
				item.tangental_y_accel = e->tangental_y_accel + ofRandom(e->tangental_y_accel_var);
				// position
				float radius = e->radius + ofRandom(e->radius_var);
				float angle = e->angle + ofRandom(e->angle_var);
				item.position = offset+radius*ofVec2f(1,1).rotateRad(angle);
				item.size = e->size + ofRandom(e->size_var);
				// uv
				ofRectangle texture_rect = m_SharedParticleUVs[(int)ofRandom(m_SharedParticleUVs.size()-1)];
				item.vertices[0].u = texture_rect.getMinX();
				item.vertices[0].v = texture_rect.getMinY();
				item.vertices[1].u = texture_rect.getMaxX();
				item.vertices[1].v = texture_rect.getMinY();
				item.vertices[2].u = texture_rect.getMaxX();
				item.vertices[2].v = texture_rect.getMaxY();
				item.vertices[3].u = texture_rect.getMinX();
				item.vertices[3].v = texture_rect.getMaxY();
				// TODO: color
				float r = e->color.r + ofRandom(e->color_var.r);
				float g = e->color.g + ofRandom(e->color_var.g);
				float b = e->color.b + ofRandom(e->color_var.b);
				float opacity = e->opacity + ofRandom(e->opacity_var);
				float intensity = e->color_intensity + ofRandom(e->color_intensity_var);
				item.vertices[0].r = r;
				item.vertices[1].r = r;
				item.vertices[2].r = r;
				item.vertices[3].r = r;
				item.vertices[0].g = g;
				item.vertices[1].g = g;
				item.vertices[2].g = g;
				item.vertices[3].g = g;
				item.vertices[0].b = b;
				item.vertices[1].b = b;
				item.vertices[2].b = b;
				item.vertices[3].b = b;
				item.vertices[0].color_intensity = intensity;
				item.vertices[1].color_intensity = intensity;
				item.vertices[2].color_intensity = intensity;
				item.vertices[3].color_intensity = intensity;
				item.vertices[0].opacity = opacity;
				item.vertices[1].opacity = opacity;
				item.vertices[2].opacity = opacity;
				item.vertices[3].opacity = opacity;
			}
			e->cooldown = e->emission_time + ofRandom(e->emission_time_var);
		}
	}
	for(int i=0;i<m_ParticleCount;i++)
	{
		ofxParticle3D& item = m_ParticlePool[i];
		item.life -= delta_time;
		while(item.life <= 0 || item.vertices[0].opacity <= 0)// life
		{
			m_ParticleCount--;
			//ofLogNotice() << "destroying "<<m_ParticleCount<<endl;
			if(m_ParticleCount == 0) break;
			ofxParticle3D p = m_ParticlePool[m_ParticleCount];
			m_ParticlePool[m_ParticleCount] = m_ParticlePool[i];
			m_ParticlePool[i] = p;
		}
		if(m_ParticleCount == 0) break;
		{// force
			// unit force
			ofVec3f u_radial, u_tangental_x, u_tangental_y;
			u_radial = item.position - item.emitter->position;
			if(u_radial != ofVec3::zero())
			{
				u_radial = u_radial.normalized();
				if(r.z != 0)
				{
					u_tangental_y = ofVec3f(0, 1, r.y/r.z);
					u_tangental_y = u_tangental_y.normalized();
				}
				else if(r.y != 0)
				{
					u_tangental_y = ofVec3f(0, r.z/r.y, 1);
					u_tangental_y = u_tangental_y.normalized();
				}
				else
				{
					u_tangental_y = ofVec3f::zero();
				}
				if(r.z != 0)
				{
					u_tangental_x = ofVec3f(1, 0, r.x/r.z);
					u_tangental_x = u_tangental_x.normalized();
				}
				else if(r.x != 0)
				{
					u_tangental_x = ofVec3f(1, 0, r.z/r.x);
					u_tangental_x = u_tangental_x.normalized();
				}
				else
				{
					u_tangental_x = ofVec3f::zero();
				}
				// real force
				item.speed += delta_time*item.accel;
				float distance = delta_time*item.speed;
				ofVec3f radial_force = distance*item.radial_accel*u_radial;
				ofVec3f tangental_x_force = distance*item.tangental_x_accel*u_tangental_x;
				ofVec3f tangental_y_force = distance*item.tangental_y_accel*u_tangental_y;
				item.position += radial_force;
				item.position += tangental_x_force;
				item.position += tangental_y_force;
			}
			item.size += delta_time*item.emitter->size_accel;
			float half_size = item.size*0.5f;
			item.vertices[0].x = item.position.x - half_size;
			item.vertices[0].y = item.position.y - half_size;
			item.vertices[0].z = m_Position.z;
			item.vertices[1].x = item.vertices[0].x + item.size;
			item.vertices[1].y = item.vertices[0].y;
			item.vertices[1].z = item.vertices[0].z;
			item.vertices[2].x = item.vertices[1].x;
			item.vertices[2].y = item.vertices[1].y + item.size;
			item.vertices[2].z = item.vertices[1].z;
			item.vertices[3].x = item.vertices[0].x;
			item.vertices[3].y = item.vertices[2].y;
			item.vertices[3].z = item.vertices[2].z;
		}
		{// TODO: color
			float opa_accel = item.emitter->opacity_accel*delta_time;
			float r_accel = item.emitter->color_accel.r*delta_time;
			float g_accel = item.emitter->color_accel.r*delta_time;
			float b_accel = item.emitter->color_accel.r*delta_time;
			item.vertices[0].r += r_accel;
			item.vertices[1].r += r_accel;
			item.vertices[2].r += r_accel;
			item.vertices[3].r += r_accel;
			item.vertices[0].g += g_accel;
			item.vertices[1].g += g_accel;
			item.vertices[2].g += g_accel;
			item.vertices[3].g += g_accel;
			item.vertices[0].b += b_accel;
			item.vertices[1].b += b_accel;
			item.vertices[2].b += b_accel;
			item.vertices[3].b += b_accel;
			item.vertices[0].opacity += opa_accel;
			item.vertices[1].opacity += opa_accel;
			item.vertices[2].opacity += opa_accel;
			item.vertices[3].opacity += opa_accel;
		}
	}
}
void ofxParticleEffect3D::SubmitChanges()
{
	m_VerticesSize = m_ParticleCount*4;
	size_t size = sizeof(ofxVertex)*4;
	for(int i=0;i<m_ParticleCount;i++)
	{
		memcpy(m_Vertices+i*4, m_ParticlePool[i].vertices, size);
	}
}
void ofxParticleEffect3D::PauseResume()
{
	m_Paused = !m_Paused;
}
bool ofxParticleEffect3D::IsPaused()
{
	return m_Paused;
}
// shared texture
vector<ofRectangle>	ofxParticleEffect3D::m_SharedParticleUVs;
ofxTexture* ofxParticleEffect3D::m_SharedParticleTexture = 0;
void ofxParticleEffect3D::LoadSharedParticleTexture()
{
	m_SharedParticleTexture = new ofxTexture();
	m_SharedParticleTexture->Load("data/particle_sheet.png");
	m_SharedParticleUVs.push_back(ofRectangle(0,0,1,1));
}