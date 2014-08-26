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
	{
		// TODO: generate base indices & uv
	}
	for(int i=0;i<m_Width;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			if(m_TileMap[i][j] == INF) continue;
			int layer = m_TileMap[i][j] / NUMBER_OF_DIFFERENT_TILE;
			if(layer >= NUMBER_OF_LAYERS) continue;
			int id = m_TileMap[i][j] % NUMBER_OF_DIFFERENT_TILE;

			ofxTile vertex_a,
				vertex_b,
				vertex_c,
				vertex_d;
			// TODO: calculate uv
			vertex_a.x = i*TILE_SIZE;
			vertex_a.y = j*TILE_SIZE;
			vertex_a.z = m_HeightMap[i][j];
			vertex_b.x = vertex_a.x + TILE_SIZE;
			vertex_b.y = vertex_a.y;
			vertex_b.z = vertex_a.z;
			vertex_c.x = vertex_b.x;
			vertex_c.y = vertex_b.y + TILE_SIZE;
			vertex_c.z = vertex_b.z;
			vertex_d.x = vertex_a.x;
			vertex_d.y = vertex_c.y;
			vertex_d.z = vertex_c.z;
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size()+0);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size()+1);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size()+2);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size()+0);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size()+2);
			m_GroundIndices[layer].push_back(m_GroundVetices[layer].size()+3);
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