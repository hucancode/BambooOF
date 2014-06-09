#include "ofxMaterialCache.h"
ofxMaterialCache::ofxMaterialCache()
{
}
ofxMaterialCache::~ofxMaterialCache()
{
	map<string, ofxMonoMaterial*>::iterator it = m_MaterialMap.begin();
	for(;it != m_MaterialMap.end();)
	{
		ofxMonoMaterial* material = (*it).second;
		delete material;
	}
}
ofxMonoMaterial* ofxMaterialCache::GetMaterial(string material_file)
{
	if(m_MaterialMap[material_file] == 0)
	{
		ofxMonoMaterial* material = new ofxMonoMaterial;
		// load file here
		m_MaterialMap[material_file] = material;
	}
	return m_MaterialMap[material_file];
}
void ofxMaterialCache::DeleteUnusedMaterial()
{
	map<string, ofxMonoMaterial*>::iterator it = m_MaterialMap.begin();
	while(it != m_MaterialMap.end())
	{
		ofxMonoMaterial* material = (*it).second;
		if(material->IsUnused())
		{
			delete material;
			it = m_MaterialMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}