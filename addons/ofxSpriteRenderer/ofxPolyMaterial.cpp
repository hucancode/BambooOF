#include "ofxPolyMaterial.h"
#include "FreeImage.h"
ofxPolyMaterial::ofxPolyMaterial()
{
	m_TextureCount = 0;
	m_TextureId = 0;
	m_TextureOrder = 0;
	m_TextureSize = 0;
	m_ShaderProgramId = 0;
	m_ShaderLocationXYZ = -1;
	m_ShaderLocationTextureCount = -1;
	m_ShaderLocationUV = 0;
	m_ShaderLocationCUV = 0;
	m_ShaderLocationTexture = 0;
	m_ReferenceCount = 0;
}
ofxPolyMaterial::~ofxPolyMaterial()
{
	// TODO: do some glDeleteWhatever here
	//glDetachShader(program, id);
	//glDeleteShader(id);
}
void ofxPolyMaterial::SetMaxTexture(const int size)
{
	if(m_TextureId || m_TextureSize || m_TextureOrder) return;
	m_TextureCount = size;
	m_TextureId = new GLuint[size];
	m_TextureSize = new GLuint[size*2];
	m_TextureOrder = new GLuint[size];
}
void ofxPolyMaterial::LoadTexturePNG(const char* texture_file, const int index)
{
	FIBITMAP* image_data = FreeImage_Load(FIF_PNG, texture_file, PNG_DEFAULT);
	unsigned int bpp = FreeImage_GetBPP(image_data);
	unsigned int width = FreeImage_GetWidth(image_data);
	unsigned int height = FreeImage_GetHeight(image_data);
	BYTE* pixel_data = FreeImage_GetBits(image_data);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		glActiveTexture(GL_TEXTURE0+index);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
		m_TextureSize[index*2] = width;
		m_TextureSize[index*2+1] = height;
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
}
bool ofxPolyMaterial::LoadShader(const char* vs_file, const char* fs_file)
{
	// generate id
	GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
	m_ShaderProgramId = glCreateProgram();
	// load data
	ofBuffer vs_buffer = ofBufferFromFile(vs_file);
	const char* vs_source = vs_buffer.getText().c_str();
	glShaderSource(vs_id, 1, &vs_source, NULL);
	ofBuffer fs_buffer = ofBufferFromFile(fs_file);
	const char* fs_source = fs_buffer.getText().c_str();
	glShaderSource(fs_id, 1, &fs_source, NULL);
	// compile shader
	glCompileShader(vs_id);
	glCompileShader(fs_id);
	GLint vs_compile_status;
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &vs_compile_status);
	GLint fs_compile_status;
	glGetShaderiv(fs_id, GL_COMPILE_STATUS, &fs_compile_status);
	if(!(vs_compile_status && fs_compile_status))
	{
#ifdef DEBUG
		GLint len = 0;
		glGetShaderiv(vs_id, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = malloc(sizeof(char) * len);
			glGetShaderInfoLog(vs_id, len, NULL, len);
			printf("Error compiling vs shader:\n%s\n", info);
			delete[] info;
		}
		glGetShaderiv(fs_id, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = malloc(sizeof(char) * len);
			glGetShaderInfoLog(fs_id, len, NULL, len);
			printf("Error compiling fs shader:\n%s\n", info);
			delete[] info;
		}
#endif
		glDeleteShader(vs_id);
		glDeleteShader(fs_id);
		return false;
	}
	// link program
	glAttachShader(m_ShaderProgramId, vs_id);
	glAttachShader(m_ShaderProgramId, fs_id);
	glLinkProgram(m_ShaderProgramId);
	GLint link_status;
	glGetProgramiv(m_ShaderProgramId, GL_LINK_STATUS, &link_status);
	if(!link_status)
	{
#ifdef DEBUG
		GLint len = 0;
		glGetProgramiv(m_ShaderProgramId, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = malloc(sizeof(char) * len);
			glGetProgramInfoLog(m_ShaderProgramId, len, NULL, info);
			printf("Error linking program:\n%s\n", infoLog);
			delete[] info;
		}
#endif
		glDeleteProgram(m_ShaderProgramId);
		return false;
	}
	return true;
}
void ofxPolyMaterial::BuildMaterial()
{
	m_ShaderLocationUV = new GLint[m_TextureCount];
	m_ShaderLocationCUV = new GLint[m_TextureCount];
	m_ShaderLocationTexture = new GLint[m_TextureCount];

	m_ShaderLocationXYZ = glGetAttribLocation(m_ShaderProgramId, "a_position");
	m_ShaderLocationTextureCount = glGetUniformLocation(m_ShaderProgramId, "u_texture_count");
	for(int i=0;i<m_TextureCount;i++)
	{
		string i_string(ofToString(i));
		m_ShaderLocationUV[i] = glGetAttribLocation(m_ShaderProgramId, string("a_uv["+i_string+"]").c_str());
		m_ShaderLocationCUV[i] = glGetAttribLocation(m_ShaderProgramId, string("a_cuv["+i_string+"]").c_str());
		m_ShaderLocationTexture[i] = glGetUniformLocation(m_ShaderProgramId, string("u_texture["+i_string+"]").c_str());
	}
}
void ofxPolyMaterial::Bind()
{
	glUseProgram(m_ShaderProgramId);
	// vertices
	glEnableVertexAttribArray(m_ShaderLocationXYZ);
	glVertexAttribPointer(m_ShaderLocationXYZ, 3, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, X));
	// tex coords
	for(int i=0;i<m_TextureCount;i++)
	{
		int id = m_TextureOrder[i];
		int idx2 = id<<1;
		glEnableVertexAttribArray(m_ShaderLocationUV[i]);
		glVertexAttribPointer(m_ShaderLocationUV[i], 2, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, UV[idx2]));
		glEnableVertexAttribArray(m_ShaderLocationCUV[i]);
		glVertexAttribPointer(m_ShaderLocationCUV[i], 2, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, CUV[idx2]));
		glUniform1i(m_ShaderLocationTexture[i], i);
	}
	// shader textures
	glUniform1i(m_ShaderLocationTextureCount, m_TextureCount);
	for(int i=0;i<m_TextureCount;i++)
	{
		int id = m_TextureOrder[i];
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, m_TextureId[id]);
	}
}
void ofxPolyMaterial::Unbind()
{
	glUseProgram(0);
	for(int i=0;i<m_TextureCount;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}