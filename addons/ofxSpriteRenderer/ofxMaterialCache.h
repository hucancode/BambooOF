#pragma once

#include "ofMain.h"
#include "ofxSpriteMaterial.h"

class ofxMaterialCache
{
public :
	static ofxMaterialCache* s_Instance;
	static void CreateInstance()
	{
		if(s_Instance) return;
		s_Instance = new ofxMaterialCache;
	}
	static ofxMaterialCache* GetInstance()
	{
		if(!s_Instance) CreateInstance();
		return s_Instance;
	}
	static void DestroyInstance()
	{
		if(!s_Instance) return;
		delete s_Instance;
		s_Instance = 0;
	}
private:
	map<string, ofxSpriteMaterial*> m_MaterialMap;
public:
	ofxMaterialCache();
	~ofxMaterialCache();
	ofxSpriteMaterial* GetMaterial(string material_file);
	void DeleteUnusedMaterial();
};