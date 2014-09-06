#include "ofxBitmapFont.h"
#include "pugixml.hpp"
#include "ofxTextureCache.h"
using namespace pugi;

ofxBitmapFont::ofxBitmapFont()
	:ofxResource()
{
	m_Texture = 0;
}
ofxBitmapFont::~ofxBitmapFont()
{
	if(m_Texture) m_Texture->DecreaseReference();
	map<char, FIBITMAP*>::iterator it;
	for (it = m_BitmapCache.begin(); it != m_BitmapCache.end(); it++)
	{
		FIBITMAP* bmp = it->second;
		delete bmp;
	}
}
bool ofxBitmapFont::Load(string xml_file)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(xml_file.c_str());
	if(!result) return false;
	xml_node root = doc.child("fontMetrics");
	if(!root) return false;
	string texture_path = root.attribute("file").as_string();
	{
		if(m_Texture)
		{
			m_Texture->DecreaseReference();
		}
		m_Texture = ofxTEXTURECACHE->GetResource(texture_path);
		m_Texture->IncreaseReference();
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
	}
	if(m_CharacterMap['?'].z == 0)
	{
		m_CharacterMap['?'] = ofVec4f(0, 0, 1, 1);
	}
	map<char, ofVec4f>::iterator it;
	for (it = m_CharacterMap.begin(); it != m_CharacterMap.end(); it++)
	{
		char key = it->first;
		ofVec4f rect = it->second;
		m_BitmapCache[key] = FreeImage_Copy(m_Texture->GetImageData(), rect.x, rect.y, rect.z, rect.w);
	}
	return true;
}
void ofxBitmapFont::IncreaseReference()
{
	ofxResource::IncreaseReference();
}
void ofxBitmapFont::DecreaseReference()
{
	ofxResource::DecreaseReference();
}
bool ofxBitmapFont::IsUnused()
{
	return ofxResource::IsUnused();
}
unsigned char ofxBitmapFont::GetFontSize()
{
	return m_FontSize;
}
ofxTexture* ofxBitmapFont::GetTexture()
{
	return m_Texture; 
}
ofVec4f ofxBitmapFont::GetCharacterRect(char character)
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
FIBITMAP* ofxBitmapFont::GetCharacterBitmap(char character)
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