#include "ofxBitmapFont.h"
#include "pugixml.hpp"
#include "ofxTextureCache.h"
using namespace pugi;

ofxBitmapFont::ofxBitmapFont()
	:ofxResource()
{
}
ofxBitmapFont::~ofxBitmapFont()
{
	map<char, ILuint>::iterator it;
	for (it = m_BitmapCache.begin(); it != m_BitmapCache.end(); it++)
	{
		ILuint id = it->second;
		ilDeleteImage(id);
	}
}
bool ofxBitmapFont::Load(string xml_file)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(xml_file.c_str());
	if(!result) return false;
	xml_node root = doc.child("fontMetrics");
	if(!root) return false;
	ILuint atlas_id;
	ilGenImages(1, &atlas_id);
	ilBindImage(atlas_id);
	ILboolean loaded = ilLoadImage(root.attribute("file").as_string());
	if(loaded != IL_TRUE)
	{
		return false;
	}
	m_FontSize = root.attribute("font_size").as_int();
	xml_node character_node = root.first_child();
	for(;character_node;character_node = character_node.next_sibling())
	{
		if(strcmp(character_node.name(), "character") != 0) break;
		int key = character_node.attribute("key").as_int();
		int x = character_node.attribute("x").as_int();
		int y = character_node.attribute("y").as_int();
		int width = character_node.attribute("width").as_int();
		int height = character_node.attribute("height").as_int();
		char c = key;
		m_CharacterMap[c] = ofVec4f(x, y, x + width, y + height);
		ILuint id;
		ilGenImages(1, &id);
		ilBindImage(atlas_id);
		ILubyte* data = new ILubyte[width*height*4];
		ilCopyPixels(x, y, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);
		ilBindImage(id);
		ilSetData(data);
		m_BitmapCache[c] = id;
	}
	if(m_CharacterMap['?'].z == 0)
	{
		m_CharacterMap['?'] = ofVec4f(0, 0, 1, 1);
		ILuint id;
		ilGenImages(1, &id);
		ilBindImage(id);
		ilTexImage(1, 1, 0, 4, IL_RGBA , IL_UNSIGNED_BYTE, NULL); 
		ilClearColour(0.0f, 0.0f, 0.0f, 0.0f);
		ilClearImage();
		m_BitmapCache['?'] = id;
	}
	ilDeleteImage(atlas_id);
	return true;
}
unsigned char ofxBitmapFont::GetFontSize()
{
	return m_FontSize;
}
ofVec4f ofxBitmapFont::GetRect(char character)
{
	if(m_CharacterMap[character].z != 0)
	{
		return m_CharacterMap[character];
	}
	else
	{
		return m_CharacterMap['?'];
	}
}
ILuint ofxBitmapFont::GetImageId(char character)
{
	if(m_BitmapCache[character])
	{
		return m_BitmapCache[character];
	}
	else
	{
		return m_BitmapCache['?'];
	}
}
ofVec2f	ofxBitmapFont::GetTextDimension(string text, unsigned char font_size)
{
	
	ofVec2f ret(0,0);
	float scale;
	if(font_size == 0)
	{
		scale = 1.0f;
	}
	else
	{
		scale = (float)font_size/m_FontSize;
	}
	for (int i = 0; i < text.size(); i++)
	{
		ofVec4f draw_region = GetRect(text[i]);
		int width = (draw_region.z - draw_region.x)*scale;
		int height = (draw_region.w - draw_region.y)*scale;
		ret.x += width;
		ret.y = max((int)ret.y, height);
	}
	return ret;
}