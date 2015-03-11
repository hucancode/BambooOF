#include "JxXmlCharacter.h"
#include "pugixml.hpp"
#include <math.h>
#define USE_FRAME_BORDER 0
JxXmlCharacter::JxXmlCharacter()
{
}
JxXmlCharacter::~JxXmlCharacter()
{
}
bool FrameCompare(JxFrame* frame_a, JxFrame* frame_b)
{
	if(frame_a->w > frame_b->w) return true;
	if(frame_a->w == frame_b->w && frame_a->h > frame_b->h) return true;
	return false;
}
bool SequenceCompare(JxSequence* sequence_a, JxSequence* sequence_b)
{
	return sequence_a->sequence_id < sequence_b->sequence_id;
}
void JxXmlCharacter::LoadOldXml(string directory, string item)
{
	{
		int index_first = directory.rbegin() - find(directory.rbegin(), directory.rend(), '/');
		int index_next = directory.rbegin() - find(directory.rbegin() + index_first + 1, directory.rend(), '/');
		m_Category = directory.substr(directory.size() + index_next, index_first - index_next - 1);
		int index_female = m_Category.find("_female");
		if(index_female != -1)
			m_Category.erase(index_female, 7);
		int index_male = m_Category.find("_male");
		if(index_male != -1)
			m_Category.erase(index_male, 5);
	}
	m_ItemName = item;
	map<string, FIBITMAP*> bitmaps;
	pugi::xml_document doc;
	string item_folder = directory + item;
	doc.load_file((item_folder+ "/"+item+".xml").c_str());
	pugi::xml_node animation_node = doc.child("animation");
	pugi::xml_node sequences_node = animation_node.child("sequences");
	pugi::xml_node sequence_node = sequences_node.first_child();
	for(;sequence_node;sequence_node = sequence_node.next_sibling())
	{
		JxSequence* sequence = new JxSequence();
		pugi::xml_node direction_node = sequence_node.first_child();
		string texture_path = direction_node.attribute("texture").as_string();
		sequence->sequence_name = texture_path;		
		texture_path = item_folder+"/"+texture_path;
		if(bitmaps[texture_path] == 0)
		{
			bitmaps[texture_path] = FreeImage_Load(FIF_PNG, texture_path.c_str(), PNG_DEFAULT);
			if(!bitmaps[texture_path])
			{
				ofLogError() << "Could not load " << texture_path.c_str();
			}
		}
		for(;direction_node;direction_node = direction_node.next_sibling())
		{
			pugi::xml_node frame_node = direction_node.first_child();
			for(;frame_node;frame_node = frame_node.next_sibling())
			{
				int rect_x = frame_node.attribute("rect_x").as_int();
				int rect_y = frame_node.attribute("rect_y").as_int();
				int rect_w = frame_node.attribute("rect_w").as_int();
				int rect_h = frame_node.attribute("rect_h").as_int();
				int offset_x = frame_node.attribute("offset_x").as_int();
				int offset_y = frame_node.attribute("offset_y").as_int();
				
				JxFrame* frame = new JxFrame();
				frame->x = rect_x - 1;
				frame->y = rect_y - 1;
				frame->w = rect_w + 2;
				frame->h = rect_h + 2;
				frame->ox = offset_x;
				frame->oy = offset_y;
				frame->image_data = FreeImage_Copy(bitmaps[texture_path], 
					rect_x, rect_y, rect_x + rect_w, rect_y + rect_h);

				frame->texture_slot = -1;
				sequence->frames.push_back(frame);
			}
		}
		m_Sequences.push_back(sequence);
	}
	CorrectSequenceName();
	GenerateSequenceCode();
	map<string, FIBITMAP*>::iterator it = bitmaps.begin();
	for(;it != bitmaps.end(); it++)
	{
		FIBITMAP* item = (*it).second;
		if(item)
		{
			FreeImage_Unload(item);
		}
	}
}
void JxXmlCharacter::Begin()
{
	m_TotalArea = 0;
	for(int i = 0;i<m_Sequences.size();i++)
	{
		for(int j=0;j<m_Sequences[i]->frames.size();j++)
		{
			m_FramesToPack.push_back(m_Sequences[i]->frames[j]);
			m_Packed.push_back(false);
			m_TotalArea += m_Sequences[i]->frames[j]->w*m_Sequences[i]->frames[j]->h;
		}
	}
	m_TotalAreaLeft = m_TotalArea;
	sort(m_FramesToPack.begin(), m_FramesToPack.end(), FrameCompare);
	m_CurrentIndex = m_Packed.size();
	m_CurrentPacker = 0;
	m_Finished = false;
}
void JxXmlCharacter::LoadXml(string directory, string item)
{
}

void JxXmlCharacter::End(string directory)
{
	if(m_Finished) return;
	sort(m_Sequences.begin(),m_Sequences.end(),SequenceCompare);
	string item_folder = directory+m_ItemName+"/";
	ofDirectory::createDirectory(item_folder, false, true);
	
	int texture_num = m_Packers.size();
	FIBITMAP** image_data = new FIBITMAP*[texture_num];
	for(int i=0;i<texture_num;i++)
	{
		image_data[i] = FreeImage_Allocate(m_Packers[i]->GetCurrentWidth(), m_Packers[i]->GetCurrentHeight(), 32);
	}
	for(int i=0;i<m_FramesToPack.size();i++)
	{
		JxFrame* frame = m_FramesToPack[i];
#if USE_FRAME_BORDER
		RGBQUAD border_color;
		border_color.rgbRed = 255;
		border_color.rgbGreen = 0;
		border_color.rgbBlue = 0;
		border_color.rgbReserved = 0;
#endif
		if(frame->rotated)
		{
			FIBITMAP* rotated_image = FreeImage_Rotate(frame->image_data, 90);
			FreeImage_Unload(frame->image_data);
			frame->image_data = rotated_image;
#if USE_FRAME_BORDER
			border_color.rgbRed = 0;
			border_color.rgbGreen = 255;
#endif
		}
#if USE_FRAME_BORDER
		FIBITMAP* dummy = FreeImage_AllocateEx(frame->w, frame->h, 32, &border_color);
#else
		FIBITMAP* dummy = FreeImage_Allocate(frame->w, frame->h, 32);
#endif
		FreeImage_Paste(dummy, frame->image_data, 1, 1, 255);
		FreeImage_Paste(image_data[frame->texture_slot], dummy, frame->x, frame->y, 255);
		FreeImage_Unload(dummy);
		FreeImage_Unload(frame->image_data);
	}
	pugi::xml_document doc;
	pugi::xml_node animation_node = doc.append_child("animation");
	animation_node.append_attribute("category") = m_Category.c_str();
	pugi::xml_node textures_node = animation_node.append_child("textures");
	textures_node.append_attribute("count") = ofToString(texture_num).c_str();
	for(int i=0;i<texture_num;i++)
	{
		string file_name = m_ItemName+ "_" + ofToString(i) +".png";
		string full_file_name = item_folder+file_name;
		FreeImage_Save(FIF_PNG, image_data[i], full_file_name.c_str());
		FreeImage_Unload(image_data[i]);
		pugi::xml_node texture_node = textures_node.append_child("texture");
		texture_node.append_attribute("path") = file_name.c_str();
		texture_node.append_attribute("id") = ofToString(i).c_str();
		delete m_Packers[i];
	}
	m_Packers.clear();
	delete[] image_data;
	pugi::xml_node sequences_node = animation_node.append_child("sequences");
	sequences_node.append_attribute("count") = ofToString(m_Sequences.size()).c_str();
	for(int i=0;i<m_Sequences.size();i++)
	{
		JxSequence* sequence = m_Sequences[i];
		pugi::xml_node sequence_node = sequences_node.append_child("sequence");
		sequence_node.append_attribute("count") = ofToString(sequence->frames.size()).c_str();
		sequence_node.append_attribute("name") = sequence->sequence_name.c_str();
		sequence_node.append_attribute("id") = ofToString((int)sequence->sequence_id).c_str();
		for(int j=0;j<m_Sequences[i]->frames.size();j++)
		{
			JxFrame* frame = sequence->frames[j];
			pugi::xml_node frame_node = sequence_node.append_child("frame");
			frame_node.append_attribute("rect_x") = ofToString(frame->x).c_str();
			frame_node.append_attribute("rect_y") = ofToString(frame->y).c_str();
			frame_node.append_attribute("rect_w") = ofToString(frame->w).c_str();
			frame_node.append_attribute("rect_h") = ofToString(frame->h).c_str();
			frame_node.append_attribute("offset_x") = ofToString(frame->ox).c_str();
			frame_node.append_attribute("offset_y") = ofToString(frame->oy).c_str();
			frame_node.append_attribute("rotated") = ofToString(frame->rotated).c_str();
			frame_node.append_attribute("texture_id") = ofToString(frame->texture_slot).c_str();
			delete frame;
		}
		sequence->frames.clear();
		delete sequence;
	}
	doc.save_file((item_folder+m_ItemName+".xml").c_str());
	m_FramesToPack.clear();
	m_Sequences.clear();
	m_Packed.clear();
	m_CurrentPacker = 0;
	m_Finished = true;
}
bool JxXmlCharacter::Update()
{
	if(m_CurrentIndex < m_Packed.size())
	{
		while(m_Packed[m_CurrentIndex])
		{
			m_CurrentIndex++;
			if(m_CurrentIndex == m_Packed.size())
			{
				break;
			}
		}
	}
	if(m_CurrentIndex == m_Packed.size())
	{
		if(m_TotalAreaLeft > 0)
		{
			if(m_CurrentPacker)
			{
				m_Packers.push_back(m_CurrentPacker);
			}
			m_CurrentPacker = new RectanglePacker();
			m_CurrentPacker->SetAutoGrow(false);
			int min_size = 100000;
			for(int i=0;i<m_FramesToPack.size();i++)
			{
				if(m_Packed[i]) continue;
				min_size = min(min_size, m_FramesToPack[i]->w);
				min_size = min(min_size, m_FramesToPack[i]->h);
			}
			m_CurrentPacker->SetRectangleSizeCap(min_size);
			m_CurrentArea = 0;
			m_CurrentIndex = 0;
			return true;
		}
		else
		{
			m_Packers.push_back(m_CurrentPacker);
			return false;// packing finished
		}
	}	
	bool pack_succeed = false;
	while(!pack_succeed)
	{
		pack_succeed = m_CurrentPacker->Insert(m_FramesToPack[m_CurrentIndex]);
		if(!pack_succeed)
		{
			if(m_CurrentArea*4 < m_TotalAreaLeft)
			{
				bool grow_succeed = m_CurrentPacker->Grow();
				if(!grow_succeed)
				{
					// this image bundle is too big, we can't handle it, so better quit
					return false;
				}
			}
			else
			{
				break;
			}
		}
	}
	if(pack_succeed)
	{
		m_FramesToPack[m_CurrentIndex]->texture_slot = m_Packers.size();
		int frame_area = m_FramesToPack[m_CurrentIndex]->w * m_FramesToPack[m_CurrentIndex]->h;
		m_TotalAreaLeft -= frame_area;
		m_CurrentArea += frame_area;
		m_Packed[m_CurrentIndex] = true;
	}
	m_CurrentIndex++;
	return true;
}
void JxXmlCharacter::CorrectSequenceName()
{
	for(int i=0;i<m_Sequences.size();i++)
	{
		string& name = m_Sequences[i]->sequence_name;
		{// remove .png
			int len = m_Sequences[i]->sequence_name.size();
			name.erase(name.begin()+(len-4), name.end());
		}
		{// remove item name
			int index = name.find_first_of('_');
			name.erase(name.begin(), name.begin()+index+1);
		}
		{
			int index = name.find(", ");
			if(index != -1)
			{
				string prefix = name.substr(0, index);
				string suffix = name.substr(index + 3, name.length() - 1);
				name = prefix + "_" +suffix;
			}
		}
		{
			int index = name.find(" ");
			if(index != -1)
			{
				string prefix = name.substr(0, index);
				string suffix = name.substr(index + 2, name.length() - 1);
				name = prefix + "_" +suffix;
			}
		}
		{
			int index = name.find("critical");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "crit";
			}
		}
		{
			int index = name.find("no_weapon_crit");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "crit_free_weapon";
			}
		}
		{
			int index = name.find("no_weapon");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "free_weapon";
			}
		}
		{
			int index = name.find("one_hand_weapon_crit");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "crit_light_weapon";
			}
		}
		{
			int index = name.find("one_hand_weapon");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "light_weapon";
			}
		}
		{
			int index = name.find("two_hand_weapon_crit");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "crit_heavy_weapon";
			}
		}
		{
			int index = name.find("two_hand_weapon");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "heavy_weapon";
			}
		}
		{
			int index = name.find("dual_weapon_crit");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "crit_dual_weapon";
			}
		}
		{
			int index = name.find("attack_flying_dagger");
			if(index != -1)
			{
				name.erase(name.begin()+index, name.end());
				name += "attack_crit_free_weapon";
			}
		}
		{
			int index = name.find("_horse");
			if(index != -1)
			{
				string prefix = name.substr(0, index);
				string suffix = name.substr(index + 6, name.length() - 1);
				name = prefix + suffix + "_ride";
			}
		}
		{
			int index = name.find("_horse");
			if(index != -1)
			{
				string prefix = name.substr(0, index);
				string suffix = name.substr(index + 6, name.length() - 1);
				name = prefix + suffix + "_ride";
			}
		}
	}
}
void JxXmlCharacter::GenerateSequenceCode()
{
	// sequence code = RRWWAAAA, 2 bit for riding, 2 bit for weapon, 4 bit for action
	for(int i=0;i<m_Sequences.size();i++)
	{
		string& name = m_Sequences[i]->sequence_name;
		m_Sequences[i]->sequence_id = 0;
		unsigned char rr = 0;
		unsigned char ww = 0;
		unsigned char aaaa = 0;
		if(name.find("ride") != -1)
		{
			rr = 1;
		}
		else
		{
			rr = 0;
		}
		if(name.find("light_weapon") != -1)
		{
			ww = 1;
		}
		else if(name.find("heavy_weapon") != -1)
		{
			ww = 2;
		}
		else if(name.find("dual_weapon") != -1)
		{
			ww = 3;
		}
		if(name.find("zen") != -1)
		{
			aaaa = 0;
		}
		else if(name.find("idle") != -1)
		{
			aaaa = 1;
		}
		else if(name.find("ready") != -1)
		{
			aaaa = 2;
		}
		else if(name.find("walk") != -1)
		{
			aaaa = 3;
		}
		else if(name.find("run") != -1)
		{
			aaaa = 4;
		}
		else if(name.find("jump") != -1)
		{
			aaaa = 5;
		}
		else if(name.find("crit") != -1)
		{
			aaaa = 6;
		}
		else if(name.find("attack") != -1)
		{
			aaaa = 7;
		}
		else if(name.find("magic") != -1)
		{
			aaaa = 8;
		}
		else if(name.find("hurt") != -1)
		{
			aaaa = 9;
		}
		else if(name.find("fall") != -1)
		{
			aaaa = 10;
		}
		m_Sequences[i]->sequence_id = m_Sequences[i]->sequence_id | aaaa;
		m_Sequences[i]->sequence_id = m_Sequences[i]->sequence_id | ww << 4;
		m_Sequences[i]->sequence_id = m_Sequences[i]->sequence_id | rr << 6;
	}
}
RectanglePacker* JxXmlCharacter::GetCurrentPacker()
{
	return m_CurrentPacker;
}
int JxXmlCharacter::GetCurrentPackerUsed()
{
	return m_Packers.size();
}
float JxXmlCharacter::GetPercentComplete()
{
	if(m_TotalArea == 0) return -1;
	return (1.0f - (float)m_TotalAreaLeft/m_TotalArea)*100;
}