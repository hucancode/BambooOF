#include "ofxBitmapFontCache.h"

ofxBitmapFontCache* ofxBitmapFontCache::s_Instance = 0;
ofxBitmapFontCache* ofxBitmapFontCache::GetInstance()
{
	return s_Instance;
}
void ofxBitmapFontCache::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxBitmapFontCache::ofxBitmapFontCache()
{
	if(s_Instance) return;
	s_Instance = this;
}
ofxBitmapFontCache::~ofxBitmapFontCache()
{
	map<string, ofxBitmapFont*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();)
	{
		ofxBitmapFont* material = (*it).second;
		delete material;
	}
}
ofxBitmapFont* ofxBitmapFontCache::GetResource(string resource_file)
{
	if(m_ResourceMap[resource_file] == 0)
	{
		ofxBitmapFont* resource = new ofxBitmapFont;
		resource->Load(resource_file);
		m_ResourceMap[resource_file] = resource;
	}
	return m_ResourceMap[resource_file];
}
void ofxBitmapFontCache::Clean()
{
	map<string, ofxBitmapFont*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		ofxBitmapFont* material = (*it).second;
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