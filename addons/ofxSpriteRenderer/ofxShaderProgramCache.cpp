#include "ofxShaderProgramCache.h"

ofxShaderProgramCache* ofxShaderProgramCache::s_Instance = 0;
ofxShaderProgramCache* ofxShaderProgramCache::GetInstance()
{
	return s_Instance;
}
void ofxShaderProgramCache::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxShaderProgramCache::ofxShaderProgramCache()
{
	if(s_Instance) return;
	s_Instance = this;
	m_DefaultResource = new ofxShaderProgram();
	m_DefaultResource->Load("default_shader");
}
ofxShaderProgramCache::~ofxShaderProgramCache()
{
	map<string, ofxShaderProgram*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();)
	{
		ofxShaderProgram* material = (*it).second;
		delete material;
	}
	delete m_DefaultResource;
}
ofxShaderProgram* ofxShaderProgramCache::GetResource(string resource_file)
{
	if(m_ResourceMap[resource_file] == 0)
	{
		ofxShaderProgram* resource = new ofxShaderProgram();
		resource->Load(resource_file);
		m_ResourceMap[resource_file] = resource;
	}
	if(m_ResourceMap[resource_file] == 0)
	{
		return m_DefaultResource;
	}
	return m_ResourceMap[resource_file];
}
bool ofxShaderProgramCache::PushResource(ofxShaderProgram* resource, string resource_name)
{
	if(!ResourceLoaded(resource_name))
	{
		m_ResourceMap[resource_name] = resource;
		return true;
	}
	return false;
}
bool ofxShaderProgramCache::ResourceLoaded(string resource_name)
{
	return m_ResourceMap[resource_name] != 0;
}
void ofxShaderProgramCache::Clean()
{
	map<string, ofxShaderProgram*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		ofxShaderProgram* shader = (*it).second;
		if(shader->IsUnused())
		{
			delete shader;
			it = m_ResourceMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}