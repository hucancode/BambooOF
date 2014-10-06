#pragma once
#include "ofMain.h"

struct RigVertex
{
	float x, y, z;
	float u, v;
	int texture;
};
class RigPart
{
	friend class RigCollectionJX;
private:
	string					m_Name;
	unsigned char			m_Part;
	bool					m_Gender;
	unsigned char			m_State;
	RigVertex				m_Vertices[4];
	float*					m_XYUVTable[70];
public:
	RigPart();
	~RigPart();
	void Load(string xml_file);
	void SetState(unsigned char state);
	void Update(float delta_time);
	void SetTextureSlot(int slot);
};