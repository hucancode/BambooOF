#pragma once
#include "ofMain.h"
#include "ofxTexture.h"
#include "JxAnimationState.h"

struct JxVertex
{
	float x, y, z;
	float u, v;
};
typedef vector<JxVertex> JxVertices;
struct JxFrameInfo
{
	float x_min, y_min;
	float x_max, y_max;
	float u_min, v_min;
	float u_max, v_max;
	unsigned char texture;
	bool rotated;
};
typedef vector<JxFrameInfo> JxFrameInfoArray;
typedef vector<JxFrameInfoArray> JxFrameInfoTable;
class JxAnimation : public ofxResource
{
private:
	ofxTextures				m_Textures;
	JxFrameInfoTable		m_FrameTable;
	JX_ANIMATION_CATEGORY	m_Category;
public:
	JxAnimation();
	~JxAnimation();
	virtual bool			Load(string xml_file);
	JX_ANIMATION_CATEGORY	GetCategory();
	JxFrameInfo				QueryFrame(JX_ANIMATION_STATE state, unsigned char frame);
	ofxTexture*				QueryTexture(JX_ANIMATION_STATE state, unsigned char frame);
};
typedef vector<JxAnimation*> JxAnimations;