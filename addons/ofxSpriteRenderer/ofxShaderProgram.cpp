#include "ofxShaderProgram.h"
#include "ofxShaderCache.h"
#include "ofxSpriteRenderer.h"

ofxShaderProgram::ofxShaderProgram()
{
	m_ShaderProgramId = glCreateProgram();
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
	glAttachShader(m_ShaderProgramId, m_VertexShader->GetShaderId());
	glAttachShader(m_ShaderProgramId, m_FragmentShader->GetShaderId());
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

	m_ShaderLocationXYZ = glGetAttribLocation(m_ShaderProgramId, "a_position");
	m_ShaderLocationProjection = glGetUniformLocation(m_ShaderProgramId, "u_projection_matrix");
	m_ShaderLocationModelView = glGetUniformLocation(m_ShaderProgramId, "u_modelview_matrix");
	m_ShaderLocationTransform = glGetUniformLocation(m_ShaderProgramId, "u_transform_matrix");
	m_ShaderLocationInvModelView = glGetUniformLocation(m_ShaderProgramId, "u_cam_inverse_matrix");

	m_ShaderLocationUV = glGetAttribLocation(m_ShaderProgramId, string("a_uv").c_str());
	m_ShaderLocationTexture = glGetUniformLocation(m_ShaderProgramId, string("u_texture").c_str());
	
	return true;
}
void ofxShaderProgram::Bind()
{
	glUseProgram(m_ShaderProgramId);
	// shader textures
	glUniform1i(m_ShaderLocationTexture,0);
	// vertices
	glEnableVertexAttribArray(m_ShaderLocationXYZ);
	glVertexAttribPointer(m_ShaderLocationXYZ, 3, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, x));
	// tex coords
	glEnableVertexAttribArray(m_ShaderLocationUV);
	glVertexAttribPointer(m_ShaderLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, u));
	// matrix
	glUniformMatrix4fv(m_ShaderLocationProjection, 1, GL_FALSE, ofxRENDERER->GetProjectionMatrix().getPtr());
	glUniformMatrix4fv(m_ShaderLocationModelView, 1, GL_FALSE, ofxRENDERER->GetModelViewMatrix().getPtr());
	glUniformMatrix4fv(m_ShaderLocationTransform, 1, GL_FALSE, ofxRENDERER->GetTransformation().getPtr());
	glUniformMatrix4fv(m_ShaderLocationInvModelView, 1, GL_FALSE, ofxRENDERER->GetInverseModelViewMatrix().getPtr());
}
void ofxShaderProgram::Unbind()
{
	glUseProgram(0);
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
bool ofxShaderProgram::IsUnused()
{
	return ofxResource::IsUnused();
}