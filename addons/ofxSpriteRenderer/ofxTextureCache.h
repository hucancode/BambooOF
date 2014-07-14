#pragma once

#include "ofMain.h"
#include "ofxTexture.h"

class ofxTextureCache
{
public :
	static ofxTextureCache* s_Instance;
	static ofxTextureCache* GetInstance();
	static void DestroyInstance();
private:
	map<string, ofxTexture*> m_ResourceMap;
public:
	ofxTextureCache();
	~ofxTextureCache();
	ofxTexture* GetResource(string resource_file);
	void Clean();
};
#define ofxTEXTURECACHE ofxTextureCache::GetInstance()