#pragma once
#include "ofMain.h"
#include "JxAnimation.h"

class JxAnimationCache
{
public :
	static JxAnimationCache*	s_Instance;
	static JxAnimationCache*	GetInstance();
	static void					DestroyInstance();
private:
	map<string, JxAnimation*>	m_ResourceMap;
public:
	JxAnimationCache();
	~JxAnimationCache();
	JxAnimation*				GetResource(string resource_file);
	void						Clean();
};
#define JXANIMATIONCACHE JxAnimationCache::GetInstance()