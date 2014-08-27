#pragma once
#include "ofMain.h"
#include "ofxTile.h"
#include "FreeImage.h"
#define INF 255
#define NUMBER_OF_LAYERS 3
#define TILE_SIZE 64
#define VERTEX_SHADER											\
"uniform mat4 u_transform_matrix;								\
attribute vec3 a_position;										\
attribute vec2 a_uv;											\
varying vec2 v_uv;												\
void main()														\
{																\
	v_uv = a_uv;												\
	gl_Position = u_transform_matrix*vec4(a_position,1.0);		\
}"
#define FRAGMENT_SHADER											\
"uniform sampler2D u_texture;									\
varying vec2 v_uv;												\
void main()														\
{																\
	vec4 color = texture2D(u_texture,v_uv);						\
    gl_FragColor = color;										\
}"

/*----------------------------------
note: 
- terrain layer doesn't like layer in photoshop
- it's kind of group, tiles in the same layer draw at the same time, using same texture
------------------------------------*/
class ofxTerrain
{
private:
	ofMatrix4x4				m_TransformMatrix;
	GLuint					m_ShaderProgramId;
	GLuint					m_VertexShaderId;
	GLuint					m_FragmentShaderId;
	GLint					m_ShaderLocationXYZ;
	GLint					m_ShaderLocationUV;
	GLint					m_ShaderLocationTexture;
	GLint					m_ShaderLocationTransform;
private:
	int						m_Width;
	int						m_Height;
	vector<vector<char> >	m_TileMap;
	vector<vector<char> >	m_LayerMap;
	vector<vector<float> >	m_HeightMap;
private:
	ofxTile					m_BaseVertices[4];
	GLuint					m_BaseVBOId;
	GLuint					m_BaseIndices[4];
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
	void					PaintTile(int x, int y);
	void					EraseTile(int x, int y);
	void					SetLayer(int x, int y, char layer);
	char					GetTileID(int x, int y);
	void					SetHeight(int x, int y, float height);
	float					GetHeight(int x, int y);
	void					BuildTileMap();
	void					RenderTiles();
public:
	bool					LoadBaseTexture(string path);
	bool					LoadGroundTexture(string path, char layer);
};