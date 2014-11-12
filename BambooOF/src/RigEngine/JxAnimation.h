#pragma once
#include "ofMain.h"
#include "ofxTexture.h"
struct JxVertex
{
	float x, y, z;
	float u, v;
	int texture;
};
struct JxFrameInfo
{
	float x_min, y_min;
	float x_max, y_max;
	float u_min, v_min;
	float u_max, v_max;
	int texture;
};
typedef vector<JxFrameInfo> JxFrameInfoArray;
typedef vector<JxFrameInfoArray> JxFrameInfoTable;
class JxAnimation
{
private:
	unsigned char			m_CurrentState;
	unsigned char			m_CurrentFrame;
	ofxTextures				m_Textures;
	JxFrameInfoTable		m_FrameTable;
public:
	JxAnimation();
	~JxAnimation();
	void Load(string xml_file);
	void SetState(unsigned char state);
	void SetFrame(unsigned char frame);
	JxFrameInfo QueryFrame();
	ofxTexture* QueryTexture();
};
