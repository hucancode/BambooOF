#include "ofxTerrain.h"
ofxTerrain::ofxTerrain()
{
	glGenBuffers(1, &m_BaseVBOId);
	glGenBuffers(1, &m_BaseIBOId);
	glGenTextures(1, &m_BaseTextureId);
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		glGenTextures(1, &m_GroundTextureId[i]);
	}
}
ofxTerrain::~ofxTerrain()
{
	glDeleteBuffers(1, &m_BaseVBOId);
	glDeleteBuffers(1, &m_BaseIBOId);
	glDeleteTextures(1, &m_BaseTextureId);
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		glDeleteTextures(1, &m_GroundTextureId[i]);
	}
}
void ofxTerrain::Initialize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		m_GroundVetices[i].clear();
		m_GroundIndices[i].clear();
		int tile = (width-1)*(height-1);
		m_GroundVetices[i].resize(tile*4);
		m_GroundIndices[i].resize(tile*6);
	}
	m_TileMap.resize(width);
	m_HeightMap.resize(width);
	for(int i=0;i<width;i++)
	{
		m_TileMap[i].resize(height);
		m_HeightMap[i].resize(height);
	}
}
void ofxTerrain::SetTile(int x, int y, short id)
{
	m_TileMap[x][y] = id;
}
short ofxTerrain::GetTile(int x, int y)
{
	return m_TileMap[x][y];
}
void ofxTerrain::SetHeight(int x, int y, float height)
{
	m_HeightMap[x][y] = height;
}
float ofxTerrain::GetHeight(int x, int y)
{
	return m_HeightMap[x][y];
}
void ofxTerrain::BuildTileMap()
{
	// TODO: implement
}
void ofxTerrain::RenderTiles()
{
	// TODO: implement
}
bool ofxTerrain::LoadBaseTexture(string path)
{
	m_BaseImage = FreeImage_Load(FIF_PNG, path.c_str(), PNG_DEFAULT);
	FreeImage_FlipVertical(m_BaseImage);
	unsigned int bpp = FreeImage_GetBPP(m_BaseImage);
	unsigned int width = FreeImage_GetWidth(m_BaseImage);
	unsigned int height = FreeImage_GetHeight(m_BaseImage);
	BYTE* pixel_data = FreeImage_GetBits(m_BaseImage);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_BaseTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param);
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			param = GL_LINEAR;
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,param);
	}
	/*delete pixel_data;
	delete image_data;*/
	return true;
}
bool ofxTerrain::LoadGroundTexture(string path, short layer)
{
	m_GroundImage[layer] = FreeImage_Load(FIF_PNG, path.c_str(), PNG_DEFAULT);
	FreeImage_FlipVertical(m_GroundImage[layer]);
	unsigned int bpp = FreeImage_GetBPP(m_GroundImage[layer]);
	unsigned int width = FreeImage_GetWidth(m_GroundImage[layer]);
	unsigned int height = FreeImage_GetHeight(m_GroundImage[layer]);
	BYTE* pixel_data = FreeImage_GetBits(m_GroundImage[layer]);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GroundTextureId[layer]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param);
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			param = GL_LINEAR;
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,param);
	}
	/*delete pixel_data;
	delete image_data;*/
	return true;
}