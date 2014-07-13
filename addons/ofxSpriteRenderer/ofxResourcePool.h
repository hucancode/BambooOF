#pragma once

#include "ofMain.h"
#include "ofxTexture.h"
#include "ofxShader.h"

template<class RESOURCE_TYPE>
class ofxResourcePool
{
public :
	static ofxResourcePool<RESOURCE_TYPE>* s_Instance;
	static void CreateInstance()
	{
		if(s_Instance) return;
		s_Instance = new ofxResourcePool<T>;
	}
	static ofxResourcePool<RESOURCE_TYPE>* GetInstance()
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
	map<string, RESOURCE_TYPE*> m_ResourceMap;
public:
	ofxResourcePool();
	~ofxResourcePool();
	RESOURCE_TYPE* GetResource(string resource_file);
	void Clean();
};
typedef ofxResourcePool<ofxTexture> ofxTextureCache;
typedef ofxResourcePool<ofxShader> ofxShaderCache;
typedef ofxResourcePool<ofxShaderProgram> ofxShaderProgramCache;
#define ofxTEXTURECACHE ofxTextureCache::GetInstance()
#define ofxSHADERCACHE ofxShaderCache::GetInstance()
#define ofxPROGRAMCACHE ofxShaderProgramCache::GetInstance()