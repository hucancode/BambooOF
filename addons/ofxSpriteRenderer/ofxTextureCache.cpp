#include "ofxTextureCache.h"

ofxTextureCache* ofxTextureCache::s_Instance = 0;
ofxTextureCache* ofxTextureCache::GetInstance()
{
	return s_Instance;
}
void ofxTextureCache::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxTextureCache::ofxTextureCache()
{
	if(s_Instance) return;
	s_Instance = this;
}
ofxTextureCache::~ofxTextureCache()
{
	map<string, ofxTexture*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();)
	{
		ofxTexture* material = (*it).second;
		delete material;
	}
}
ofxTexture* ofxTextureCache::GetResource(string resource_file)
{
	if(m_ResourceMap[resource_file] == 0)
	{
		ofxTexture* resource = new ofxTexture;
		resource->Load(resource_file);
		m_ResourceMap[resource_file] = resource;
	}
	return m_ResourceMap[resource_file];
}
bool ofxTextureCache::PushResource(ofxTexture* resource, string resource_name)
{
	if(m_ResourceMap[resource_name] == 0)
	{
		m_ResourceMap[resource_name] = resource;
		return true;
	}
	return false;
}
void ofxTextureCache::Clean()
{
	map<string, ofxTexture*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		ofxTexture* material = (*it).second;
		if(material->IsUnused())
		{
			delete material;
			it = m_ResourceMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}