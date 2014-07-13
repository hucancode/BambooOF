#pragma once
#include "ofxShader.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteRenderer.h"
#include "ofxResourcePool.h"
ofxShader::ofxShader()
{
	GLuint m_ShaderId = 0;
}
ofxShader::~ofxShader()
{
}
bool ofxShader::Load(string shader_path)
{
	GLenum type;
	string extension = shader_path.substr(shader_path.length()-4,4);
	if(extension == "vert")
	{
		Load(shader_path, GL_VERTEX_SHADER);
	}
	else if(extension == "frag")
	{
		Load(shader_path, GL_FRAGMENT_SHADER);
	}
}
bool ofxShader::Load(string shader_path, GLenum type)
{
	m_ShaderId = glCreateShader(type);
	ofBuffer shader_file = ofBufferFromFile(shader_path);
	string shader_source = shader_file.getText();
	const char* shader_source_raw = shader_source.c_str();
	int shader_source_length = shader_source.size()+1;
	glShaderSource(m_ShaderId, 1, &shader_source_raw, &shader_source_length);
	glCompileShader(m_ShaderId);
	GLint compile_status;
	glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &compile_status);
	if(!compile_status)
	{
#ifdef _DEBUG
		GLint len = 0;
		glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = new char[len];
			glGetShaderInfoLog(m_ShaderId, len, NULL, info);
			printf("Error compiling shader[%s]:\n%s\n", shader_path.c_str(), info);
			delete[] info;
		}
#endif
		glDeleteShader(m_ShaderId);
		return false;
	}
}
void ofxShader::IncreaseReference()
{
	ofxResource::IncreaseReference();
}
void ofxShader::DecreaseReference()
{
	ofxResource::DecreaseReference();
}
ofxShaderProgram::ofxShaderProgram()
{
	m_ShaderProgramId = glCreateProgram();
	m_TextureCount = 1;
}
ofxShaderProgram::~ofxShaderProgram()
{
	//glDetachShader(program, id);
	//glDeleteShader(id);
}
bool ofxShaderProgram::Load(string path)
{
	return Load(path+".vert", path+".frag");
}
bool ofxShaderProgram::Load(string vs_path, string fs_path)
{
	m_VertexShader = ofxSHADERCACHE->GetResource(vs_path);
	m_FragmentShader = ofxSHADERCACHE->GetResource(fs_path);
	// link program
	glAttachShader(m_ShaderProgramId, m_VertexShader->m_ShaderId);
	glAttachShader(m_ShaderProgramId, m_FragmentShader->m_ShaderId);
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
			printf("Error linking program[%s,%s]:\n%s\n", vs_path.c_str(), fs_path.c_str(), info);
			delete[] info;
		}
#endif
		glDeleteProgram(m_ShaderProgramId);
		return false;
	}
	m_ShaderLocationUV = new GLint[m_TextureCount];
	m_ShaderLocationTexture = new GLint[m_TextureCount];

	m_ShaderLocationXYZ = glGetAttribLocation(m_ShaderProgramId, "a_position");
	m_ShaderLocationProjection = glGetUniformLocation(m_ShaderProgramId, "u_projection_matrix");
	m_ShaderLocationModelView = glGetUniformLocation(m_ShaderProgramId, "u_modelview_matrix");
	m_ShaderLocationTransform = glGetUniformLocation(m_ShaderProgramId, "u_transform_matrix");
	m_ShaderLocationInvModelView = glGetUniformLocation(m_ShaderProgramId, "u_cam_inverse_matrix");

	m_ShaderLocationTextureCount = glGetUniformLocation(m_ShaderProgramId, "u_texture_count");
	for(int i=0;i<m_TextureCount;i++)
	{
		string i_string(ofToString(i));
		m_ShaderLocationUV[i] = glGetAttribLocation(m_ShaderProgramId, string("a_uv["+i_string+"]").c_str());
		m_ShaderLocationTexture[i] = glGetUniformLocation(m_ShaderProgramId, string("u_texture["+i_string+"]").c_str());
	}
	return true;
}
void ofxShaderProgram::SetOrder(const int texture_id, const int order)
{
	m_TextureOrder[order] = texture_id;
}
void ofxShaderProgram::Bind()
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
		glVertexAttribPointer(m_ShaderLocationUV[i], 4, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, UV[idx4]));
		glUniform1i(m_ShaderLocationTexture[i], i);
	}
	// matrix
	glUniformMatrix4fv(m_ShaderLocationProjection, 1, GL_FALSE, ofxRENDERER->GetProjectionMatrix().getPtr());
	glUniformMatrix4fv(m_ShaderLocationModelView, 1, GL_FALSE, ofxRENDERER->GetModelViewMatrix().getPtr());
	glUniformMatrix4fv(m_ShaderLocationTransform, 1, GL_FALSE, ofxRENDERER->GetTransformation().getPtr());
	glUniformMatrix4fv(m_ShaderLocationInvModelView, 1, GL_FALSE, ofxRENDERER->GetInverseModelViewMatrix().getPtr());
	// shader textures
	glUniform1i(m_ShaderLocationTextureCount, m_TextureCount);
	for(int i=0;i<m_TextureCount;i++)
	{
		glUniform1i(m_ShaderLocationTexture[i],i);
	}
}
void ofxShaderProgram::Unbind()
{
}
void ofxShaderProgram::IncreaseReference()
{
	ofxResource::IncreaseReference();
	m_VertexShader->IncreaseReference();
	m_FragmentShader->IncreaseReference();
}
void ofxShaderProgram::DecreaseReference()
{
	ofxResource::DecreaseReference();
	m_VertexShader->DecreaseReference();
	m_FragmentShader->DecreaseReference();
}