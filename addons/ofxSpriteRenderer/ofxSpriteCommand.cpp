#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
GLuint* ofxSpriteCommand::m_Indices = 0;
GLuint ofxSpriteCommand::m_IBOId;
void ofxSpriteCommand::GenerateSharedIndices()
{
	int number_of_vertices = COMMAND_CAPACITY*4;
	int number_of_indices = COMMAND_CAPACITY*6;
	m_Indices = new GLuint[number_of_indices];
	glGenBuffers(1, &m_IBOId);
	int index = 0;
	for(unsigned int i=0;i<number_of_vertices;i+=4)
	{
		m_Indices[index++] = i+0;
		m_Indices[index++] = i+1;
		m_Indices[index++] = i+2;
		m_Indices[index++] = i+0;
		m_Indices[index++] = i+2;
		m_Indices[index++] = i+3;
	}
}
void ofxSpriteCommand::DeleteSharedIndices()
{
	glDeleteBuffers(1, &m_IBOId);
	delete[] m_Indices;
}

ofxSpriteCommand::ofxSpriteCommand()
{
	glGenBuffers(1, &m_VBOId);
	m_Texture = 0;
	m_Shader = 0;
	m_Vertices = new ofxVertex[COMMAND_CAPACITY*4];
	m_VerticesSize = 0;
	m_IndicesSize = 0;
}
ofxSpriteCommand::~ofxSpriteCommand()
{
	delete[] m_Vertices;
	glDeleteBuffers(1, &m_VBOId);
}
void ofxSpriteCommand::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ofxVertex)*m_VerticesSize, &m_Vertices[0], GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*m_IndicesSize, &m_Indices[0], GL_STATIC_DRAW);
	m_Shader->Bind();
	m_Texture->Bind();

	glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_Shader->Unbind();
	m_Texture->Unbind();
}
void ofxSpriteCommand::PushSprite(ofxSpriteQuad* sprite)
{
	memcpy(&m_Vertices[m_VerticesSize], &sprite->m_Vertex[0], sizeof(ofxVertex)*4);
	m_VerticesSize += 4;
	m_IndicesSize += 6;
}
ofxTexture* ofxSpriteCommand::GetTexture()
{
	return m_Texture;
}
ofxShaderProgram* ofxSpriteCommand::GetShader()
{
	return m_Shader;
}
void ofxSpriteCommand::SetTexture(ofxTexture* texture)
{
	m_Texture = texture;
}
void ofxSpriteCommand::SetShader(ofxShaderProgram* shader)
{
	m_Shader = shader;
}