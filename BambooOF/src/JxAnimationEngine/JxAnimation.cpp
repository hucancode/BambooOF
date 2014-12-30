#include "JxAnimation.h"
#include "ofxTextureCache.h"
#include "pugixml.hpp"
#include "ofMain.h"
#include "JxAnimationState.h"

JxAnimation::JxAnimation()
{
	m_CurrentState = JX_ANIMATION_STATE_ZEN;
	m_CurrentFrame = 0;
}
JxAnimation::~JxAnimation()
{
}
void JxAnimation::Load(string xml_file)
{
	string animation_folder = ofFilePath::getEnclosingDirectory(xml_file, false);
	pugi::xml_document doc;
	doc.load_file(xml_file.c_str());
	pugi::xml_node animation_node = doc.child("animation");
	{// load textures
		pugi::xml_node textures_node = animation_node.child("textures");
		int texture_count = textures_node.attribute("count").as_int();
		m_Textures.resize(texture_count);
		pugi::xml_node texture_node = textures_node.child("texture");
		for(int i=0;i<texture_count && texture_node;i++, texture_node = texture_node.next_sibling())
		{
			const char* path = texture_node.attribute("path").as_string();
			int id = texture_node.attribute("id").as_int();
			m_Textures[id] = ofxTEXTURECACHE->GetResource(animation_folder+"/"+path);
		}
	}
	{// load sequences information
		pugi::xml_node sequences_node = animation_node.child("sequences");
		int sequence_count = sequences_node.attribute("count").as_int();
		m_FrameTable.resize(JX_ANIMATION_STATE_MAX);
		pugi::xml_node sequence_node = sequences_node.child("sequence");
		for(int i=0;i<sequence_count;i++)
		{
			int frame_count = sequence_node.attribute("count").as_int();
			string sequence_name = sequence_node.attribute("name").as_string();
			int sequence_id = sequence_node.attribute("id").as_int();
			m_FrameTable[sequence_id].resize(frame_count);
			pugi::xml_node frame_node = sequence_node.child("frame");
			for(int j=0;j<frame_count;j++)
			{
				JxFrameInfo& frame = m_FrameTable[sequence_id][j];

				int x = frame_node.attribute("rect_x").as_int();
				int y = frame_node.attribute("rect_y").as_int();
				int w = frame_node.attribute("rect_w").as_int();
				int h = frame_node.attribute("rect_h").as_int();
				int ox = frame_node.attribute("offset_x").as_int();
				int oy = frame_node.attribute("offset_y").as_int();
				bool rotated = frame_node.attribute("rotated").as_bool();
				int texture_id = frame_node.attribute("texture_id").as_int();

				float uw = m_Textures[texture_id]->GetUnitWidth();
				float uh = m_Textures[texture_id]->GetUnitHeight();

				frame.u_min = x*uw;
				frame.v_min = y*uh;
				frame.u_max = frame.u_min + w*uw;
				frame.v_max = frame.v_min + h*uh;
				frame.x_min = 256-ox;
				frame.y_min = 256-oy;
				if(rotated)
				{
					frame.x_max = frame.x_min - h;
					frame.y_max = frame.y_min - w;
				}
				else
				{
					frame.x_max = frame.x_min - w;
					frame.y_max = frame.y_min - h;
				}
				frame.rotated = rotated;
				frame.texture = texture_id;
				frame_node = frame_node.next_sibling();
			}
			sequence_node = sequence_node.next_sibling();
		}
	}
}
void JxAnimation::SetTextureSlot(unsigned char slot)
{
	m_TextureSlot = slot;
}
unsigned char JxAnimation::GetTextureSlot()
{
	return m_TextureSlot;
}
void JxAnimation::SetState(JX_ANIMATION_STATE state)
{
	m_CurrentState = state;
}
void JxAnimation::SetFrame(unsigned char frame)
{
	m_CurrentFrame = frame;
}
JxFrameInfo JxAnimation::QueryFrame()
{
	return m_FrameTable[(int)m_CurrentState][m_CurrentFrame];
}
ofxTexture* JxAnimation::QueryTexture()
{
	return m_Textures[m_FrameTable[(int)m_CurrentState][m_CurrentFrame].texture];
}