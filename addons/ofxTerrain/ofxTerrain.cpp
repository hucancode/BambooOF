#include "ofxTerrain.h"
ofxTerrain::ofxTerrain()
{
	{
		glGenBuffers(1, &m_BaseVBOId);
		glGenTextures(1, &m_BaseTextureId);
		m_BaseImageId = ilGenImage();
		ilGenImages(NUMBER_OF_LAYERS, m_GroundImageId);
		for(int i=0;i<NUMBER_OF_LAYERS;i++)
		{
			glGenBuffers(1, &m_GroundVBOId[i]);
			glGenTextures(1, &m_GroundTextureId[i]);
		}
	}
	{
		m_VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		const char* shader_source_raw = VERTEX_SHADER;
		int shader_source_length = strlen(shader_source_raw)+1;
		glShaderSource(m_VertexShaderId, 1, &shader_source_raw, &shader_source_length);
		glCompileShader(m_VertexShaderId);
		GLint compile_status;
		glGetShaderiv(m_VertexShaderId, GL_COMPILE_STATUS, &compile_status);
		if(!compile_status)
		{
#ifdef _DEBUG
			GLint len = 0;
			glGetShaderiv(m_VertexShaderId, GL_INFO_LOG_LENGTH, &len);
			if(len > 1)
			{
				char* info = new char[len];
				glGetShaderInfoLog(m_VertexShaderId, len, NULL, info);
				printf("Error compiling terrain shader:\n%s\n", info);
				delete[] info;
			}
#endif
			glDeleteShader(m_VertexShaderId);
		}
	}
	{
		m_FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		const char* shader_source_raw = FRAGMENT_SHADER;
		int shader_source_length = strlen(shader_source_raw)+1;
		glShaderSource(m_FragmentShaderId, 1, &shader_source_raw, &shader_source_length);
		glCompileShader(m_FragmentShaderId);
		GLint compile_status;
		glGetShaderiv(m_FragmentShaderId, GL_COMPILE_STATUS, &compile_status);
		if(!compile_status)
		{
#ifdef _DEBUG
			GLint len = 0;
			glGetShaderiv(m_FragmentShaderId, GL_INFO_LOG_LENGTH, &len);
			if(len > 1)
			{
				char* info = new char[len];
				glGetShaderInfoLog(m_FragmentShaderId, len, NULL, info);
				printf("Error compiling terrain shader:\n%s\n", info);
				delete[] info;
			}
#endif
			glDeleteShader(m_FragmentShaderId);
		}
	}
	{
		m_ShaderProgramId = glCreateProgram();
		glAttachShader(m_ShaderProgramId, m_VertexShaderId);
		glAttachShader(m_ShaderProgramId, m_FragmentShaderId);
		glLinkProgram(m_ShaderProgramId);
	}
	{
		m_ShaderLocationXYZ = glGetAttribLocation(m_ShaderProgramId, "a_position");
		m_ShaderLocationTransform = glGetUniformLocation(m_ShaderProgramId, "u_transform_matrix");
		m_ShaderLocationUV = glGetAttribLocation(m_ShaderProgramId, string("a_uv").c_str());
		m_ShaderLocationTexture = glGetUniformLocation(m_ShaderProgramId, string("u_texture").c_str());
	}
}
ofxTerrain::~ofxTerrain()
{
	glDeleteBuffers(1, &m_BaseVBOId);
	glDeleteTextures(1, &m_BaseTextureId);
	ilDeleteImage(m_BaseImageId); 
	for(int i=0;i<NUMBER_OF_LAYERS;i++)
	{
		glDeleteBuffers(1, &m_GroundVBOId[i]);
		glDeleteTextures(1, &m_GroundTextureId[i]);
		ilDeleteImage(m_GroundImageId[i]); 
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
	m_TileMap.resize(width);
	m_LayerMap.resize(width);
	m_HeightMap.resize(width);
	for(int i=0;i<width;i++)
	{
		m_TileMap[i].resize(height);
		m_LayerMap[i].resize(height);
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
		m_TileMap[x][y] = m_TileMap[x][y] | 8;//1000
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
		m_TileMap[x][y-1] = m_TileMap[x][y-1] & 11;//1011
	}
	// Bottom Right
	{
		m_TileMap[x][y] = m_TileMap[x][y] & 7;//0111
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
	const float noise_proof = 0.004f;// this will fix texture interpolation bug
	const float uv_width = 0.25f - noise_proof*2;
	const float uv_height = uv_width;
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
		m_BaseVertices[1].u = m_BaseVertices[0].u + (m_Width+1)*0.25;
		m_BaseVertices[1].v = m_BaseVertices[0].v;
		m_BaseVertices[2].u = m_BaseVertices[1].u;
		m_BaseVertices[2].v = m_BaseVertices[1].v + (m_Height+1)*0.25;
		m_BaseVertices[3].u = m_BaseVertices[0].u;
		m_BaseVertices[3].v = m_BaseVertices[2].v;
	}
	for(int i=0; i<NUMBER_OF_LAYERS; i++)
	{
		m_GroundVetices[i].clear();
	}
	for(int i=0; i<m_Width; i++)
	{
		for(int j=0; j<m_Height; j++)
		{
			if(m_TileMap[i][j] == 0) continue;
			char layer = m_LayerMap[i][j];
			if(layer >= NUMBER_OF_LAYERS) continue;
			char id = m_TileMap[i][j];
			ofxTile vertex_a,
				vertex_b,
				vertex_c,
				vertex_d;
			{
				char x = id & 3;
				char y = id >> 2;
				vertex_a.u = x*0.25f + noise_proof;
				vertex_a.v = y*0.25f + noise_proof;
				vertex_b.u = vertex_a.u + uv_width;
				vertex_b.v = vertex_a.v;
				vertex_c.u = vertex_b.u;
				vertex_c.v = vertex_b.v + uv_height;
				vertex_d.u = vertex_a.u;
				vertex_d.v = vertex_c.v;
			}
			{
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
			}
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
	glUniform1i(m_ShaderLocationTexture, 0);
	glUniformMatrix4fv(m_ShaderLocationTransform, 1, GL_FALSE, m_TransformMatrix.getPtr());
	glEnableVertexAttribArray(m_ShaderLocationXYZ);
	glEnableVertexAttribArray(m_ShaderLocationUV);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BaseVBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ofxTile)*4, m_BaseVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(m_ShaderLocationXYZ, 3, GL_FLOAT, GL_FALSE, sizeof(ofxTile), (GLvoid*) offsetof( ofxTile, x));
		glVertexAttribPointer(m_ShaderLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ofxTile), (GLvoid*) offsetof( ofxTile, u));
		glBindTexture(GL_TEXTURE_2D, m_BaseTextureId);

		glDrawArrays(GL_QUADS, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	for(int i=0; i<NUMBER_OF_LAYERS; i++)
	{
		if(m_GroundVetices[i].size() == 0) continue;
		glBindBuffer(GL_ARRAY_BUFFER, m_GroundVBOId[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ofxTile)*m_GroundVetices[i].size(), &m_GroundVetices[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(m_ShaderLocationXYZ, 3, GL_FLOAT, GL_FALSE, sizeof(ofxTile), (GLvoid*) offsetof( ofxTile, x));
		glVertexAttribPointer(m_ShaderLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ofxTile), (GLvoid*) offsetof( ofxTile, u));
		glBindTexture(GL_TEXTURE_2D, m_GroundTextureId[i]);

		glDrawArrays(GL_QUADS, 0, m_GroundVetices[i].size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}
bool ofxTerrain::LoadBaseTexture(string path)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_BaseTextureId);
	ilBindImage(m_BaseImageId);
	ILboolean loaded = ilLoadImage(path.c_str());
	if (loaded == IL_FALSE)
	{
		ILenum error = ilGetError();
		ofLogError() <<"DevIL failed to load image "<<path.c_str()<<endl<<"error code "<<error;
		return false; 
	}
	{
		ILinfo info;
		iluGetImageInfo(&info);
		ILubyte* pixel_data = ilGetData();
		GLenum format = info.Bpp==3?GL_RGB:GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, info.Width, info.Height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
	}
	{
		GLint param = GL_REPEAT;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, param);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, param);
	}
	{
		GLint param = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	return true;
}
bool ofxTerrain::LoadGroundTexture(string path, char layer)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GroundTextureId[layer]);
	ilBindImage(m_GroundImageId[layer]);
	ILboolean loaded = ilLoadImage(path.c_str());
	if (loaded == IL_FALSE)
	{
		ILenum error = ilGetError();
		ofLogError() <<"DevIL failed to load image "<<path.c_str()<<endl<<"error code "<<error;
		return false; 
	}
	{
		ILinfo info;
		iluGetImageInfo(&info);
		ILubyte* pixel_data = ilGetData();
		GLenum format = info.Bpp==3?GL_RGB:GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, info.Width, info.Height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	}
	{
		GLint param = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		
	}
	/*delete pixel_data;
	delete image_data;*/
	return true;
}