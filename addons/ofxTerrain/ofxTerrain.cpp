#include "ofxTerrain.h"
ofxTerrain::ofxTerrain()
{
	{
		glGenBuffers(1, &m_BaseVBOId);
		glGenBuffers(1, &m_BaseIBOId);
		glGenTextures(1, &m_BaseTextureId);
		for(int i=0;i<NUMBER_OF_LAYERS;i++)
		{
			glGenBuffers(1, &m_GroundVBOId[i]);
			glGenBuffers(1, &m_GroundIBOId[i]);
			glGenTextures(1, &m_GroundTextureId[i]);
		}
	}
	{
		m_VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		const char* shader_source_raw = VERTEX_SHADER;
		int shader_source_length = strlen(shader_source_raw)+1;
		glShaderSource(m_VertexShaderId, 1, &shader_source_raw, &shader_source_length);
		glCompileShader(m_VertexShaderId);
	}
	{
		m_FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		const char* shader_source_raw = FRAGMENT_SHADER;
		int shader_source_length = strlen(shader_source_raw)+1;
		glShaderSource(m_FragmentShaderId, 1, &shader_source_raw, &shader_source_length);
		glCompileShader(m_FragmentShaderId);
	}
	{
		m_ShaderProgramId = glCreateProgram();
		glAttachShader(m_ShaderProgramId, m_VertexShaderId);
		glAttachShader(m_ShaderProgramId, m_FragmentShaderId);
		glLinkProgram(m_ShaderProgramId);
	}
}
ofxTerrain::~ofxTerrain()
{
	glDeleteBuffers(1, &m_BaseVBOId);
	glDeleteBuffers(1, &m_BaseIBOId);
	glDeleteTextures(1, &m_BaseTextureId);
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		glDeleteBuffers(1, &m_GroundVBOId[i]);
		glDeleteBuffers(1, &m_GroundIBOId[i]);
		glDeleteTextures(1, &m_GroundTextureId[i]);
	}
	{
		glDetachShader(m_ShaderProgramId, m_FragmentShaderId);
		glDeleteShader(m_FragmentShaderId);
		glDetachShader(m_ShaderProgramId, m_VertexShaderId);
		glDeleteShader(m_VertexShaderId);
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
void ofxTerrain::PaintTile(int x, int y)
{
	// Bottom Left
	if(x > 0)
	{
		m_TileMap[x-1][y] = m_TileMap[x-1][y] | 1;//0001
	}
	// Top Left
	if(x > 0 && y > 0)
	{
		m_TileMap[x-1][y-1] = m_TileMap[x-1][y-1] | 2;//0010
	}
	// Top Right
	if(y > 0)
	{
		m_TileMap[x][y-1] = m_TileMap[x][y-1] | 4;//0100
	}
	// Bottom Right
	{
		m_TileMap[x][y-1] = m_TileMap[x][y-1] | 8;//1000
	}
}
void ofxTerrain::SetLayer(int x, int y, char layer)
{
	m_LayerMap[x][y] = layer;
}
void ofxTerrain::EraseTile(int x, int y)
{
	// Bottom Left
	if(x > 0)
	{
		m_TileMap[x-1][y] = m_TileMap[x-1][y] & 14;//1110
	}
	// Top Left
	if(x > 0 && y > 0)
	{
		m_TileMap[x-1][y-1] = m_TileMap[x-1][y-1] & 13;//1101
	}
	// Top Right
	if(y > 0)
	{
		m_TileMap[x][y-1] = m_TileMap[x][y-1] & 11;//0100
	}
	// Bottom Right
	{
		m_TileMap[x][y-1] = m_TileMap[x][y-1] & 7;//0111
	}
}
char ofxTerrain::GetTileID(int x, int y)
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
	{
		m_BaseVertices[0].x = 0.0f;
		m_BaseVertices[0].y = 0.0f;
		m_BaseVertices[0].z = 0.0f;
		m_BaseVertices[1].x = m_BaseVertices[0].x + (m_Width+1)*TILE_SIZE;
		m_BaseVertices[1].y = m_BaseVertices[0].y;
		m_BaseVertices[1].z = m_BaseVertices[0].z;
		m_BaseVertices[2].x = m_BaseVertices[1].x;
		m_BaseVertices[2].y = m_BaseVertices[1].y;
		m_BaseVertices[2].z = m_BaseVertices[1].z + (m_Height+1)*TILE_SIZE;
		m_BaseVertices[3].x = m_BaseVertices[0].x;
		m_BaseVertices[3].y = m_BaseVertices[2].y;
		m_BaseVertices[3].z = m_BaseVertices[2].z;
		m_BaseVertices[0].u = 0.0f;
		m_BaseVertices[0].v = 0.0f;
		m_BaseVertices[1].u = m_BaseVertices[0].u + (m_Width+1);
		m_BaseVertices[1].v = m_BaseVertices[0].v;
		m_BaseVertices[2].u = m_BaseVertices[1].u;
		m_BaseVertices[2].v = m_BaseVertices[1].v + (m_Height+1);
		m_BaseVertices[3].u = m_BaseVertices[0].u;
		m_BaseVertices[3].v = m_BaseVertices[2].v;
	}
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		m_GroundIndices[i].clear();
		m_GroundVetices[i].clear();
	}
	for(int i=0;i<m_Width;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			if(m_TileMap[i][j] == INF) continue;
			char layer = m_LayerMap[i][j];
			if(layer >= NUMBER_OF_LAYERS) continue;
			char id = m_TileMap[i][j];

			ofxTile vertex_a,
				vertex_b,
				vertex_c,
				vertex_d;

			vertex_a.u = (id%4)*0.25f;
			vertex_a.v = (id/4)*0.25f;// this calculation is not optimized to improve readability
			vertex_b.u = vertex_a.u + 0.25f;
			vertex_b.v = vertex_a.v;
			vertex_c.u = vertex_b.u;
			vertex_c.v = vertex_b.v + 0.25f;
			vertex_d.u = vertex_a.u;
			vertex_d.v = vertex_c.v;

			vertex_a.x = i*TILE_SIZE;
			vertex_a.y = m_HeightMap[i][j];
			vertex_a.z = j*TILE_SIZE;
			vertex_b.x = vertex_a.x + TILE_SIZE;
			vertex_b.y = vertex_a.y;
			vertex_b.z = vertex_a.z;
			vertex_c.x = vertex_b.x;
			vertex_c.y = vertex_b.y;
			vertex_c.z = vertex_b.z + TILE_SIZE;
			vertex_d.x = vertex_a.x;
			vertex_d.y = vertex_c.y;
			vertex_d.z = vertex_c.z;
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size() + 0);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size() + 1);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size() + 2);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size() + 0);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size() + 2);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size() + 3);
			m_GroundVetices[layer].push_back(vertex_a);
			m_GroundVetices[layer].push_back(vertex_b);
			m_GroundVetices[layer].push_back(vertex_c);
			m_GroundVetices[layer].push_back(vertex_d);
		}
	}
}
void ofxTerrain::RenderTiles()
{
	m_TransformMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW)*ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
	glDepthMask(GL_FALSE);
	glUseProgram(m_ShaderProgramId);
	glUniform1i(m_ShaderLocationTexture,0);
	glEnableVertexAttribArray(m_ShaderLocationXYZ);
	glVertexAttribPointer(m_ShaderLocationXYZ, 3, GL_FLOAT, GL_FALSE, sizeof(ofxTile), (GLvoid*) offsetof( ofxTile, x));
	glEnableVertexAttribArray(m_ShaderLocationUV);
	glVertexAttribPointer(m_ShaderLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ofxTile), (GLvoid*) offsetof( ofxTile, u));
	glUniformMatrix4fv(m_ShaderLocationTransform, 1, GL_FALSE, m_TransformMatrix.getPtr());
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BaseVBOId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BaseIBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ofxTile)*4, &m_BaseVertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, &m_BaseIndices[0], GL_STATIC_DRAW);
		glBindTexture(GL_TEXTURE_2D, m_BaseTextureId);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_GroundVBOId[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GroundIBOId[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ofxTile)*m_GroundVetices[i].size(), &m_GroundVetices[i][0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_GroundIndices[i].size(), &m_GroundIndices[i][0], GL_STATIC_DRAW);
		glBindTexture(GL_TEXTURE_2D, m_GroundTextureId[i]);

		glDrawElements(GL_TRIANGLES, m_GroundIndices[i].size(), GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);
	glDepthMask(GL_TRUE);
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
		GLint param = GL_REPEAT;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, param);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, param);
	}
	{
		GLint param = GL_REPEAT;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			param = GL_LINEAR;
		}
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, param);
	}
	/*delete pixel_data;
	delete image_data;*/
	return true;
}
bool ofxTerrain::LoadGroundTexture(string path, char layer)
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