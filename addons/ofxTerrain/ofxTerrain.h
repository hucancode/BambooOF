#pragma once
#include "ofMain.h"
#include "ofxTile.h"

class ofxTerrain
{
private:
	int						m_Width;
	int						m_Height;
	int**					m_TileMap;
	float**					m_HeightMap;
private:
	GLsizei					m_VerticesSize;
	ofxTile*				m_Tiles;
	GLuint					m_VBOId;
	GLsizei					m_IndicesSize;
	GLuint*					m_Indices;
	GLuint					m_IBOId;
public:
	ofxTerrain();
	~ofxTerrain();
	void					Initialize(int width, int height);
	void					SetTile(int x, int y, int id);
	int						GetTile(int x, int y);
	void					SetHeight(int x, int y, float height);
	float					GetHeight(int x, int y);
	void					BuildTileMap();
	void					RenderTiles();
};