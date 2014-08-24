#pragma once
#include "ofMain.h"
#include "ofxTile.h"
#include "FreeImage.h"

class ofxTerrain
{
private:
	int						m_Width;
	int						m_Height;
	vector<vector<short> >	m_TileMap;
	vector<vector<float> >	m_HeightMap;
private:
	vector<ofxTile>			m_Tiles;
	GLuint					m_VBOId;
	vector<GLuint>			m_Indices;
	GLuint					m_IBOId;
private:
	FIBITMAP*				m_SurfaceImageData;
	GLuint					m_TextureId;
public:
	ofxTerrain();
	~ofxTerrain();
	void					Initialize(int width, int height);
	void					SetTile(int x, int y, short id);
	short					GetTile(int x, int y);
	void					SetHeight(int x, int y, float height);
	float					GetHeight(int x, int y);
	void					BuildTileMap();
	void					RenderTiles();
public:
	bool					LoadSurfaceTexture(string path);
};