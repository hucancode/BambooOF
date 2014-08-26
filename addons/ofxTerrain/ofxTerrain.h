#pragma once
#include "ofMain.h"
#include "ofxTile.h"
#include "FreeImage.h"
#define INF 10000
#define NUMBER_OF_LAYERS 3

class ofxTerrain
{
private:
	int						m_Width;
	int						m_Height;
	vector<vector<short> >	m_TileMap;
	vector<vector<float> >	m_HeightMap;
private:
	GLuint					m_BaseVBOId;
	GLuint					m_BaseIBOId;
	vector<ofxTile>			m_GroundVetices[NUMBER_OF_LAYERS];
	GLuint					m_GroundVBOId[NUMBER_OF_LAYERS];
	vector<GLuint>			m_GroundIndices[NUMBER_OF_LAYERS];
	GLuint					m_GroundIBOId[NUMBER_OF_LAYERS];
private:
	FIBITMAP*				m_BaseImage;
	GLuint					m_BaseTextureId;
	FIBITMAP*				m_GroundImage[NUMBER_OF_LAYERS];
	GLuint					m_GroundTextureId[NUMBER_OF_LAYERS];
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
	bool					LoadBaseTexture(string path);
	bool					LoadGroundTexture(string path, short layer);
};