#include "ofxSpriteAnimation.h"
ofxSpriteAnimation::ofxSpriteAnimation()
{
	m_FrameCount = 0;
	m_TextureRectTable = 0;
	m_SpriteRectTable = 0;
	m_SequenceCount = 0;
	m_SequenceBegin = 0;
	m_SequenceEnd = 0;
	m_SequenceTime = 0;
	m_FrameTime = 0;
	m_AnimationTime = 0.0f;
	m_FrameIndex = 0;
	m_Paused = false;
}
ofxSpriteAnimation::~ofxSpriteAnimation()
{
	// remember to delete pointer
}
void ofxSpriteAnimation::SetSequenceTime(unsigned short sequence_index, float time)
{
	m_SequenceTime[sequence_index] = time;
}
void ofxSpriteAnimation::SetFrameTime(unsigned short sequence_index, float time)
{
	m_FrameTime[sequence_index] = time;
}
void ofxSpriteAnimation::SetFrameCount(unsigned short count)
{
	m_FrameCount = count;
	m_TextureRectTable = new unsigned short*[count];
	for(int i=0; i < count;i++)
	{
		m_TextureRectTable[i] = new unsigned short[4];
	}
	m_SpriteRectTable = new unsigned short*[count];
	for(int i=0; i < count;i++)
	{
		m_SpriteRectTable[i] = new unsigned short[4];
	}
}
void ofxSpriteAnimation::SetFrameData(unsigned short index, 
		unsigned short texture_rect_x, unsigned short texture_rect_y, unsigned short texture_rect_w, unsigned short texture_rect_h,
		unsigned short sprite_rect_x, unsigned short sprite_rect_y, unsigned short sprite_rect_w, unsigned short sprite_rect_h)
{
	m_TextureRectTable[index][0] = texture_rect_x;
	m_TextureRectTable[index][1] = texture_rect_y;
	m_TextureRectTable[index][2] = texture_rect_w;
	m_TextureRectTable[index][3] = texture_rect_h;
	m_SpriteRectTable[index][0] = sprite_rect_x;
	m_SpriteRectTable[index][1] = sprite_rect_y;
	m_SpriteRectTable[index][2] = sprite_rect_w;
	m_SpriteRectTable[index][3] = sprite_rect_h;
}
void ofxSpriteAnimation::SetSequence(unsigned short index)
{
	m_SequenceIndex = index;
	m_FrameIndex = m_SequenceBegin[m_SequenceIndex];
}
void ofxSpriteAnimation::SetSequenceCount(unsigned short count)
{
	m_SequenceCount = count;
	m_SequenceBegin = new unsigned short[count];
	m_SequenceEnd = new unsigned short[count];
}
void ofxSpriteAnimation::SetSequenceData(unsigned short index, unsigned short begin, unsigned short end)
{
	m_SequenceBegin[index] = begin;
	m_SequenceEnd[index] = end;
}
void ofxSpriteAnimation::Update(const float delta_time)
{
	m_AnimationTime += delta_time;
	if(m_AnimationTime >= m_FrameTime[m_SequenceIndex])
	{
		m_AnimationTime -= m_FrameTime[m_SequenceIndex];
		if(m_FrameIndex == m_SequenceEnd[m_SequenceIndex])
		{
			m_FrameIndex = m_SequenceBegin[m_SequenceIndex];
		}
		else
		{
			m_FrameIndex++;
		}
	}
}
void ofxSpriteAnimation::PauseResume()
{
	m_Paused = !m_Paused;
}
bool ofxSpriteAnimation::IsPaused()
{
	return m_Paused;
}