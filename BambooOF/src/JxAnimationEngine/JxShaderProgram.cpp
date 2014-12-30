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
	m_UniformMap["u_texture"]				= glGetUniformLocation(m_ProgramId, "u_texture");
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
	// --------------
	glUniform1i						(m_UniformMap["u_texture"],				0);
	if(s_LastProgramId != m_ProgramId)
	{
		glUniformMatrix4fv			(m_UniformMap["u_modelview_matrix"],	1, GL_FALSE, 
			ofxRENDERER->GetModelViewMatrix().getPtr());
		glUniformMatrix4fv			(m_UniformMap["u_projection_matrix"],	1, GL_FALSE, 
			ofxRENDERER->GetProjectionMatrix().getPtr());
		glUniformMatrix4fv			(m_UniformMap["u_transform_matrix"],	1, GL_FALSE, 
			ofxRENDERER->GetTransformation().getPtr());
		glUniformMatrix4fv			(m_UniformMap["u_cam_inverse_matrix"],	1, GL_FALSE, 
			ofxRENDERER->GetInverseModelViewMatrix().getPtr());
	}
}
void JxShaderProgram::SetTextureSlot(GLuint slot)
{
	glUniform1i(m_UniformMap["u_texture"], slot);
}