#include "ofxTerrain.h"
ofxTerrain::ofxTerrain()
{
	glGenBuffers(1, &m_VBOId);
	glGenBuffers(1, &m_IBOId);
	glGenTextures(1, &m_TextureId);
}
ofxTerrain::~ofxTerrain()
{
	glDeleteBuffers(1, &m_VBOId);
	glDeleteBuffers(1, &m_IBOId);
	glDeleteTextures(1, &m_TextureId);
}
void ofxTerrain::Initialize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_Tiles.clear();
	m_Indices.clear();
	int tile = (width-1)*(height-1);
	m_Tiles.resize(tile*4);
	m_Indices.resize(tile*6);
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
bool ofxTerrain::LoadSurfaceTexture(string path)
{
	m_SurfaceImageData = FreeImage_Load(FIF_PNG, path.c_str(), PNG_DEFAULT);
	FreeImage_FlipVertical(m_SurfaceImageData);
	unsigned int bpp = FreeImage_GetBPP(m_SurfaceImageData);
	unsigned int width = FreeImage_GetWidth(m_SurfaceImageData);
	unsigned int height = FreeImage_GetHeight(m_SurfaceImageData);
	BYTE* pixel_data = FreeImage_GetBits(m_SurfaceImageData);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
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