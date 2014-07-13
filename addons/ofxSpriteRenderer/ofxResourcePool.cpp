#include "ofxResourcePool.h"
template<class RESOURCE_TYPE>
ofxResourcePool<RESOURCE_TYPE>::ofxResourcePool()
{
}
template<class RESOURCE_TYPE>
ofxResourcePool<RESOURCE_TYPE>::~ofxResourcePool()
{
	map<string, RESOURCE_TYPE*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();)
	{
		RESOURCE_TYPE* material = (*it).second;
		delete material;
	}
}
template<class RESOURCE_TYPE>
RESOURCE_TYPE* ofxResourcePool<RESOURCE_TYPE>::GetResource(string resource_file)
{
	if(m_ResourceMap[resource_file] == 0)
	{
		RESOURCE_TYPE* resource = new RESOURCE_TYPE;
		resource->Load(resource_file);
		m_ResourceMap[resource_file] = resource;
	}
	return m_ResourceMap[material_file];
}
template<class RESOURCE_TYPE>
void ofxResourcePool<RESOURCE_TYPE>::Clean()
{
	map<string, RESOURCE_TYPE*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		RESOURCE_TYPE* material = (*it).second;
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