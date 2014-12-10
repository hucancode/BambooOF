#include "ofMain.h"
#include "ofxSpriteRenderer.h"
#include "JxShaderProgram.h"
#include "JxAnimation.h"


JxShaderProgram::JxShaderProgram()
	:ofxShaderProgram()
{
}
JxShaderProgram::~JxShaderProgram()
{
}
void JxShaderProgram::BuildReferenceMap()
{
	m_AttributeMap["a_position"]			= glGetAttribLocation(m_ProgramId, "a_position");
	m_AttributeMap["a_uv"]					= glGetAttribLocation(m_ProgramId, "a_uv");
	m_AttributeMap["a_texture_id"]			= glGetAttribLocation(m_ProgramId, "a_texture_id");
	m_UniformMap["u_textures[0]"]			= glGetUniformLocation(m_ProgramId, "u_textures[0]");
	m_UniformMap["u_textures[1]"]			= glGetUniformLocation(m_ProgramId, "u_textures[1]");
	m_UniformMap["u_textures[2]"]			= glGetUniformLocation(m_ProgramId, "u_textures[2]");
	m_UniformMap["u_textures[3]"]			= glGetUniformLocation(m_ProgramId, "u_textures[3]");
	m_UniformMap["u_textures[4]"]			= glGetUniformLocation(m_ProgramId, "u_textures[4]");
	m_UniformMap["u_textures[5]"]			= glGetUniformLocation(m_ProgramId, "u_textures[5]");
	m_UniformMap["u_textures[6]"]			= glGetUniformLocation(m_ProgramId, "u_textures[6]");
	m_UniformMap["u_textures[7]"]			= glGetUniformLocation(m_ProgramId, "u_textures[7]");
	m_UniformMap["u_textures[8]"]			= glGetUniformLocation(m_ProgramId, "u_textures[8]");
	m_UniformMap["u_textures[9]"]			= glGetUniformLocation(m_ProgramId, "u_textures[9]");
	m_UniformMap["u_textures[10]"]			= glGetUniformLocation(m_ProgramId, "u_textures[10]");
	m_UniformMap["u_modelview_matrix"]		= glGetUniformLocation(m_ProgramId, "u_modelview_matrix");
	m_UniformMap["u_projection_matrix"]		= glGetUniformLocation(m_ProgramId, "u_projection_matrix");
	m_UniformMap["u_transform_matrix"]		= glGetUniformLocation(m_ProgramId, "u_transform_matrix");
	m_UniformMap["u_cam_inverse_matrix"]	= glGetUniformLocation(m_ProgramId, "u_cam_inverse_matrix");
}
void JxShaderProgram::BindReferenceMap()
{
	int stride = sizeof(JxVertex);
	glEnableVertexAttribArray	(m_AttributeMap["a_position"]);
	glVertexAttribPointer		(m_AttributeMap["a_position"],			3, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(JxVertex, x));
	glEnableVertexAttribArray	(m_AttributeMap["a_uv"]);
	glVertexAttribPointer		(m_AttributeMap["a_uv"],				2, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(JxVertex, u));
	glEnableVertexAttribArray	(m_AttributeMap["a_texture_id"]);
	glVertexAttribPointer		(m_AttributeMap["a_texture_id"],		1, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(JxVertex, texture));
	// --------------
	glUniform1i					(m_UniformMap["u_textures[0]"],			0);
	glUniform1i					(m_UniformMap["u_textures[1]"],			1);
	glUniform1i					(m_UniformMap["u_textures[2]"],			2);
	glUniform1i					(m_UniformMap["u_textures[3]"],			3);
	glUniform1i					(m_UniformMap["u_textures[4]"],			4);
	glUniform1i					(m_UniformMap["u_textures[5]"],			5);
	glUniform1i					(m_UniformMap["u_textures[6]"],			6);
	glUniform1i					(m_UniformMap["u_textures[7]"],			7);
	glUniform1i					(m_UniformMap["u_textures[8]"],			8);
	glUniform1i					(m_UniformMap["u_textures[9]"],			9);
	glUniform1i					(m_UniformMap["u_textures[10]"],		10);
	glUniformMatrix4fv			(m_UniformMap["u_modelview_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetModelViewMatrix().getPtr());
	glUniformMatrix4fv			(m_UniformMap["u_projection_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetProjectionMatrix().getPtr());
	glUniformMatrix4fv			(m_UniformMap["u_transform_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetTransformation().getPtr());
	glUniformMatrix4fv			(m_UniformMap["u_cam_inverse_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetInverseModelViewMatrix().getPtr());
}