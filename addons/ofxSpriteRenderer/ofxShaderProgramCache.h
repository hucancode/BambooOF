#pragma once

#include "ofMain.h"
#include "ofxShaderProgram.h"

class ofxShaderProgramCache
{
public :
	static ofxShaderProgramCache*	s_Instance;
	static ofxShaderProgramCache*	GetInstance();
	static void						DestroyInstance();
private:
	map<string, ofxShaderProgram*>	m_ResourceMap;
	ofxShaderProgram*				m_DefaultResource;
public:
	ofxShaderProgramCache();
	~ofxShaderProgramCache();
	ofxShaderProgram*				GetResource(string resource_file);
	bool							PushResource(ofxShaderProgram* resource, string resource_name);
	bool							ResourceLoaded(string resource_name);
	void							Clean();
};
#define ofxSHADERPROGRAMCACHE ofxShaderProgramCache::GetInstance()