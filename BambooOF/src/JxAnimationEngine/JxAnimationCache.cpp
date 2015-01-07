#include "JxAnimationCache.h"
JxAnimationCache* JxAnimationCache::s_Instance = 0;
JxAnimationCache* JxAnimationCache::GetInstance()
{
	return s_Instance;
}
void JxAnimationCache::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
JxAnimationCache::JxAnimationCache()
{
	if(s_Instance) return;
	s_Instance = this;
}
JxAnimationCache::~JxAnimationCache()
{
	map<string, JxAnimation*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();it++)
	{
		JxAnimation* material = (*it).second;
		delete material;
	}
}
JxAnimation* JxAnimationCache::GetResource(string resource_file)
{
	int a = 10;
	if(m_ResourceMap[resource_file] == 0)
	{
		JxAnimation* resource = new JxAnimation();
		if(resource->Load(resource_file))
		{
			m_ResourceMap[resource_file] = resource;
		}
	}
	return m_ResourceMap[resource_file];
}
void JxAnimationCache::Clean()
{
	map<string, JxAnimation*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		JxAnimation* material = (*it).second;
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