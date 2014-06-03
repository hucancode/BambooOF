#include "ofxMaterialCache.h"
ofxMaterialCache::ofxMaterialCache()
{
}
ofxMaterialCache::~ofxMaterialCache()
{
	map<string, ofxSpriteMaterial*>::iterator it = m_MaterialMap.begin();
	for(;it != m_MaterialMap.end();)
	{
		ofxSpriteMaterial* material = (*it).second;
		delete material;
	}
}
ofxSpriteMaterial* ofxMaterialCache::GetMaterial(string material_file)
{
	if(m_MaterialMap[material_file] == 0)
	{
		ofxSpriteMaterial* material = new ofxSpriteMaterial;
		// load file here
		m_MaterialMap[material_file] = material;
	}
	return m_MaterialMap[material_file];
}
void ofxMaterialCache::DeleteUnusedMaterial()
{
	map<string, ofxSpriteMaterial*>::iterator it = m_MaterialMap.begin();
	for(;it != m_MaterialMap.end();)
	{
		ofxSpriteMaterial* material = (*it).second;
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