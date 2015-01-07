#include "JxAnimation.h"
#include "ofxTextureCache.h"
#include "pugixml.hpp"
#include "ofMain.h"
#include "JxAnimationState.h"

JxAnimation::JxAnimation()
{
}
JxAnimation::~JxAnimation()
{
}
bool JxAnimation::Load(string xml_file)
{
	using namespace pugi;
	string animation_folder = ofFilePath::getEnclosingDirectory(xml_file, false);
	xml_document doc;
	xml_parse_result ret = doc.load_file(xml_file.c_str());
	if(!ret)
	{
		return false;
	}
	xml_node animation_node = doc.child("animation");
	{
		string category = animation_node.attribute("category").as_string();
		if(category == "character_helm")
		{
			m_Category = JX_ANIMATION_CATEGORY_HELM;
		}
		else if(category == "character_cloth")
		{
			m_Category = JX_ANIMATION_CATEGORY_CLOTH;
		}
		else if(category == "character_arml")
		{
			m_Category = JX_ANIMATION_CATEGORY_ARML;
		}
		else if(category == "character_armr")
		{
			m_Category = JX_ANIMATION_CATEGORY_ARMR;
		}
		else if(category == "weapon_light")
		{
			m_Category = JX_ANIMATION_CATEGORY_WEAPON_PRIMARY;
		}
		else if(category == "weapon_heavy")
		{
			m_Category = JX_ANIMATION_CATEGORY_WEAPON_PRIMARY;
		}
		else if(category == "weapon_duall")
		{
			m_Category = JX_ANIMATION_CATEGORY_WEAPON_PRIMARY;
		}
		else if(category == "weapon_dualr")
		{
			m_Category = JX_ANIMATION_CATEGORY_WEAPON_SECONDARY;
		}
		else if(category == "horse_head")
		{
			m_Category = JX_ANIMATION_CATEGORY_HORSE_HEAD;
		}
		else if(category == "horse_back")
		{
			m_Category = JX_ANIMATION_CATEGORY_HORSE_BACK;
		}
		else if(category == "horse_tail")
		{
			m_Category = JX_ANIMATION_CATEGORY_HORSE_TAIL;
		}
		else
		{
			m_Category = JX_ANIMATION_CATEGORY_HELM;
		}
	}
	{// load textures
		xml_node textures_node = animation_node.child("textures");
		int texture_count = textures_node.attribute("count").as_int();
		m_Textures.resize(texture_count);
		xml_node texture_node = textures_node.child("texture");
		for(int i=0;i<texture_count && texture_node;i++, texture_node = texture_node.next_sibling())
		{
			const char* path = texture_node.attribute("path").as_string();
			int id = texture_node.attribute("id").as_int();
			m_Textures[id] = ofxTEXTURECACHE->GetResource(animation_folder+"/"+path);
		}
	}
	{// load sequences information
		xml_node sequences_node = animation_node.child("sequences");
		int sequence_count = sequences_node.attribute("count").as_int();
		m_FrameTable.resize(JX_ANIMATION_STATE_MAX);
		xml_node sequence_node = sequences_node.child("sequence");
		for(int i=0;i<sequence_count;i++)
		{
			int frame_count = sequence_node.attribute("count").as_int();
			string sequence_name = sequence_node.attribute("name").as_string();
			int sequence_id = sequence_node.attribute("id").as_int();
			m_FrameTable[sequence_id].resize(frame_count);
			xml_node frame_node = sequence_node.child("frame");
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
	return true;
}
JX_ANIMATION_CATEGORY JxAnimation::GetCategory()
{
	return m_Category;
}
JxFrameInfo JxAnimation::QueryFrame(JX_ANIMATION_STATE state, unsigned char frame)
{
	if(m_Category == JX_ANIMATION_CATEGORY_WEAPON_PRIMARY || 
		m_Category == JX_ANIMATION_CATEGORY_WEAPON_SECONDARY)
	{
		state = (JX_ANIMATION_STATE)(state & JX_VOID_WEAPON_FLAG);
	}
	else if(m_Category == JX_ANIMATION_CATEGORY_HORSE_HEAD || 
		m_Category == JX_ANIMATION_CATEGORY_HORSE_BACK || 
		m_Category == JX_ANIMATION_CATEGORY_HORSE_TAIL)
	{
		state = (JX_ANIMATION_STATE)(state & JX_VOID_WEAPON_FLAG & JX_VOID_HORSE_FLAG);
	}
	return m_FrameTable[(int)state][(int)frame];
}
ofxTexture* JxAnimation::QueryTexture(JX_ANIMATION_STATE state, unsigned char frame)
{
	return m_Textures[QueryFrame(state, frame).texture];
}