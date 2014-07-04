#include "ofxPolyMaterial.h"
#include "ofxSpriteRenderer.h"

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
	m_ShaderLocationTransform = 0;
	m_ShaderLocationInvModelView = 0;
	m_ReferenceCount = 0;
}
ofxPolyMaterial::~ofxPolyMaterial()
{
	// TODO: do some glDeleteWhatever here
	//glDetachShader(program, id);
	//glDeleteShader(id);
	//FreeImage_Unload(m_ImageData[index];
}
void ofxPolyMaterial::SetMaxTexture(const int size)
{
	if(m_TextureId || m_TextureSize || m_TextureOrder) return;
	m_TextureCount = size;
	m_TextureId = new GLuint[size];
	m_TextureSize = new ofVec2f[size];
	m_TextureOrder = new GLuint[size];
	m_ImageData = new FIBITMAP*[size];
}
void ofxPolyMaterial::LoadTexturePNG(const int index, const char* texture_file)
{
	glGenTextures(1, &m_TextureId[index]);
	m_ImageData[index] = FreeImage_Load(FIF_PNG, texture_file, PNG_DEFAULT);
	FreeImage_FlipVertical(m_ImageData[index]);
	unsigned int bpp = FreeImage_GetBPP(m_ImageData[index]);
	unsigned int width = FreeImage_GetWidth(m_ImageData[index]);
	unsigned int height = FreeImage_GetHeight(m_ImageData[index]);
	BYTE* pixel_data = FreeImage_GetBits(m_ImageData[index]);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		glActiveTexture(GL_TEXTURE0+index);
		glBindTexture(GL_TEXTURE_2D, m_TextureId[index]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
		m_TextureSize[index].x = width;
		m_TextureSize[index].y = height;
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
	string vs_string = vs_buffer.getText();
	const char* vs_source = vs_string.c_str();
	int vs_size = vs_buffer.getText().size()+1;
	glShaderSource(vs_id, 1, &vs_source, &vs_size);
	ofBuffer fs_buffer = ofBufferFromFile(fs_file);
	string fs_string = fs_buffer.getText();
	const char* fs_source = fs_string.c_str();
	int fs_size = fs_buffer.getText().size()+1;
	glShaderSource(fs_id, 1, &fs_source, &fs_size);
	// compile shader
	glCompileShader(vs_id);
	glCompileShader(fs_id);
	GLint vs_compile_status;
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &vs_compile_status);
	GLint fs_compile_status;
	glGetShaderiv(fs_id, GL_COMPILE_STATUS, &fs_compile_status);
	if(!(vs_compile_status && fs_compile_status))
	{
#ifdef _DEBUG
		GLint len = 0;
		glGetShaderiv(vs_id, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = new char[len];
			glGetShaderInfoLog(vs_id, len, NULL, info);
			printf("Error compiling vs shader:\n%s\n", info);
			delete[] info;
		}
		glGetShaderiv(fs_id, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = new char[len];
			glGetShaderInfoLog(fs_id, len, NULL, info);
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
#ifdef _DEBUG
		GLint len = 0;
		glGetProgramiv(m_ShaderProgramId, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = new char[len];
			glGetProgramInfoLog(m_ShaderProgramId, len, NULL, info);
			printf("Error linking program:\n%s\n", info);
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
	
	m_ShaderLocationTransform = glGetUniformLocation(m_ShaderProgramId, "u_transform_matrix");
	m_ShaderLocationInvModelView = glGetUniformLocation(m_ShaderProgramId, "u_cam_inverse_matrix");

	m_ShaderLocationTextureCount = glGetUniformLocation(m_ShaderProgramId, "u_texture_count");
	for(int i=0;i<m_TextureCount;i++)
	{
		string i_string(ofToString(i));
		m_ShaderLocationUV[i] = glGetAttribLocation(m_ShaderProgramId, string("a_uv["+i_string+"]").c_str());
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
		int idx4 = id<<2;
		glEnableVertexAttribArray(m_ShaderLocationUV[i]);
		glVertexAttribPointer(m_ShaderLocationUV[i], 2, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, UV[idx4]));
		glUniform1i(m_ShaderLocationTexture[i], i);
	}
	// matrix
	glUniformMatrix4fv(m_ShaderLocationTransform, 1, GL_FALSE, ofxRENDERER->GetTransformation().getPtr());
	glUniformMatrix4fv(m_ShaderLocationInvModelView, 1, GL_FALSE, ofxRENDERER->GetCamera()->GetInverseModelViewMatrix().getPtr());
	// shader textures
	glUniform1i(m_ShaderLocationTextureCount, m_TextureCount);
	for(int i=0;i<m_TextureCount;i++)
	{
		int id = m_TextureOrder[i];
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, m_TextureId[id]);
		glUniform1i(m_ShaderLocationTexture[i],i);
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