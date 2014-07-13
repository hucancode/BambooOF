#pragma once
#include "ofMain.h"
class ofxResource
{
protected:
	unsigned int m_ReferenceCount;
public:
	ofxResource()
	{
		m_ReferenceCount = 0;
	}
	virtual ~ofxResource()
	{
	}
	virtual bool Load(string resource_file);
	virtual void IncreaseReference()
	{
		m_ReferenceCount++;
	}
	virtual void DecreaseReference()
	{
		m_ReferenceCount--;
	}
	virtual bool IsUnused()
	{
		return m_ReferenceCount == 0;
	}
};