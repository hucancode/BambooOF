#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
vector<GLushort> ofxSpriteCommand::m_Indices;
GLuint ofxSpriteCommand::m_IBOId;
void ofxSpriteCommand::GenerateSharedIndices(unsigned short number_of_quad)
{
	if(number_of_quad > 6000) return;
	number_of_quad *= 4;
	glGenBuffers(1, &m_IBOId);
	for(unsigned int i=0;i<number_of_quad;i+=4)
	{
		m_Indices.push_back(i+0);
		m_Indices.push_back(i+1);
		m_Indices.push_back(i+2);
		m_Indices.push_back(i+0);
		m_Indices.push_back(i+2);
		m_Indices.push_back(i+3);
	}
}
void ofxSpriteCommand::DeleteSharedIndices()
{
	glDeleteBuffers(1, &m_IBOId);
	m_Indices.clear();
}

ofxSpriteCommand::ofxSpriteCommand()
{
	glGenBuffers(1, &m_VBOId);
	//glGenBuffers(1, &m_IBOId);
	m_VisibleSpriteCount = 0;
	m_IndicesSize = 0;
	m_FirstSpriteIndex = -1;
	m_LastSpriteIndex = -1;
	m_DistanceMin = 0;
	m_DistanceMax = 0;
	m_Shader = 0;
	m_Status = COMMAND_STATUS_UNITED;
	m_NextSibling = 0;
	m_PrevSibling = 0;
}
ofxSpriteCommand::~ofxSpriteCommand()
{
	glDeleteBuffers(1, &m_VBOId);
	//glDeleteBuffers(1, &m_IBOId);
}
void ofxSpriteCommand::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ofxVertex)*m_Vertices.size(), &m_Vertices[0], GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*m_IndicesSize, &m_Indices[0], GL_STATIC_DRAW);
	m_Shader->Bind(m_Textures.size());
	for(int i=0;i<m_Textures.size();i++)
	{
		m_Textures[i]->Bind(i);
	}
}
void ofxSpriteCommand::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// more unbind available, but is it need?
}
void ofxSpriteCommand::Render()
{
	if(m_VisibleSpriteCount == 0) return;
	Bind();
	glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_SHORT, 0);
	Unbind();
}
void ofxSpriteCommand::PushSprite(ofxSpriteQuad* sprite)
{
	float distance = sprite->GetWorldPosition().z;
	ofxVertex vertexA, vertexB, vertexC, vertexD;
	sprite->m_IndexInCommand = m_Vertices.size();
	sprite->m_ParentCommand = this;
	if(m_FirstSpriteIndex == -1)
	{
		m_FirstSpriteIndex = m_LastSpriteIndex = sprite->m_IndexInRenderer;
		m_DistanceMin = m_DistanceMax = distance;
	}
	if(sprite->m_IndexInRenderer < m_FirstSpriteIndex)
	{
		m_FirstSpriteIndex = sprite->m_IndexInRenderer;
	}
	else if(sprite->m_IndexInRenderer > m_LastSpriteIndex)
	{
		m_LastSpriteIndex = sprite->m_IndexInRenderer;
	}
	if(distance < m_DistanceMin)
	{
		m_DistanceMin = distance;
	}
	else if(distance > m_DistanceMax)
	{
		m_DistanceMax = distance;
	}
	m_Vertices.push_back(vertexA);
	m_Vertices.push_back(vertexB);
	m_Vertices.push_back(vertexC);
	m_Vertices.push_back(vertexD);
	m_IndicesSize += 6;
	m_VisibleSprite.push_back(true);
	m_VisibleSpriteCount++;
	sprite->m_PositionChange = true;
	sprite->m_UVChange = true;
	UpdateSprite(sprite);
	
}
void ofxSpriteCommand::EraseSprite(ofxSpriteQuad* sprite)
{
	if(m_Status != COMMAND_STATUS_EXPANDED || m_Status == COMMAND_STATUS_DISMISSED) return;
	unsigned int index = sprite->m_IndexInCommand;
	bool visible = sprite->m_Visible && sprite->m_Visibility == QUAD_VISIBILITY_IN_SCREEN;
	unsigned int index4 = index*0.25;
	if(visible && !m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount++;
		m_VisibleSprite[index4] = true;
	}
	else if(!visible && m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount--;
		m_VisibleSprite[index4] = false;
	}

	m_Vertices.erase(m_Vertices.begin() + index);
	m_Vertices.erase(m_Vertices.begin() + index+1);
	m_Vertices.erase(m_Vertices.begin() + index+2);
	m_Vertices.erase(m_Vertices.begin() + index+3);
}
void ofxSpriteCommand::UpdateSprite(ofxSpriteQuad* sprite)
{
	if(sprite->m_PositionChange)
	{
		float distance = sprite->GetWorldPosition().z;
		if(distance > m_DistanceMax || distance < m_DistanceMin)
		{
			if(m_Status != COMMAND_STATUS_EXPANDED)
			{
				bool expand = false;
				if(m_NextSibling)
				{
					if(distance > m_NextSibling->m_DistanceMin)
					{
						expand = true;
					}
				}
				if(m_PrevSibling && !expand)
				{
					if(distance < m_PrevSibling->m_DistanceMax)
					{
						expand = true;
					}
				}
				if(expand)
				{
					m_Status = COMMAND_STATUS_EXPANDED;
				}
			}
			if(m_DistanceMax < distance) m_DistanceMax = distance;
			if(m_DistanceMin > distance) m_DistanceMin = distance;
		}
		if(m_Status == COMMAND_STATUS_UNITED && m_FirstSpriteIndex != m_LastSpriteIndex)
		{
			bool dismiss = false;
			if(sprite->NextSibling())
			{
				float next_distance = sprite->NextSibling()->GetWorldPosition().z;
				if(distance > next_distance)
				{
					dismiss = true;
				}
			}
			if(sprite->NextSibling() && !dismiss)
			{
				float prev_distance = sprite->NextSibling()->GetWorldPosition().z;
				if(distance < prev_distance)
				{
					dismiss = true;
				}
			}
			if(dismiss)
			{
				m_Status = COMMAND_STATUS_DISMISSED;
			}
		}
	}
	else if(sprite->m_MaterialChange)
	{
		m_Status = COMMAND_STATUS_DISMISSED;
	}
	if(m_Status == COMMAND_STATUS_EXPANDED || m_Status == COMMAND_STATUS_DISMISSED)
	{
		return;
	}
	unsigned int index = sprite->m_IndexInCommand;
	bool visible = sprite->m_Visible && sprite->m_Visibility == QUAD_VISIBILITY_IN_SCREEN;
	unsigned int index4 = index*0.25;
	if(visible && !m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount++;
		m_VisibleSprite[index4] = true;
	}
	else if(!visible && m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount--;
		m_VisibleSprite[index4] = false;
	}
	ofxVertex *vertexA, *vertexB, *vertexC, *vertexD;
	vertexA = &m_Vertices[index];
	vertexB = &m_Vertices[index+1];
	vertexC = &m_Vertices[index+2];
	vertexD = &m_Vertices[index+3];
	if(sprite->m_PositionChange)
	{
		vertexA->X = sprite->m_glPosition[0].x;
		vertexA->Y = sprite->m_glPosition[0].y;
		vertexA->Z = sprite->m_glPosition[0].z;
		vertexB->X = sprite->m_glPosition[1].x;
		vertexB->Y = sprite->m_glPosition[1].y;
		vertexB->Z = sprite->m_glPosition[1].z;
		vertexC->X = sprite->m_glPosition[2].x;
		vertexC->Y = sprite->m_glPosition[2].y;
		vertexC->Z = sprite->m_glPosition[2].z;
		vertexD->X = sprite->m_glPosition[3].x;
		vertexD->Y = sprite->m_glPosition[3].y;
		vertexD->Z = sprite->m_glPosition[3].z;
	}
	if(sprite->m_UVChange)
	{
		for(int i = 0;i < sprite->m_Textures.size();i++)
		{
			int x = i<<2;
			int y = x+1;
			int z = y+1;
			int w = z+1;
			vertexA->UV[x] = sprite->m_glUV[i].x;
			vertexA->UV[y] = sprite->m_glUV[i].y;
			vertexB->UV[x] = sprite->m_glUV[i].z;
			vertexB->UV[y] = sprite->m_glUV[i].y;
			vertexC->UV[x] = sprite->m_glUV[i].z;
			vertexC->UV[y] = sprite->m_glUV[i].w;
			vertexD->UV[x] = sprite->m_glUV[i].x;
			vertexD->UV[y] = sprite->m_glUV[i].w;
			vertexA->UV[z] = sprite->m_glCUV[i].x;
			vertexA->UV[w] = sprite->m_glCUV[i].y;
			vertexB->UV[z] = sprite->m_glCUV[i].z;
			vertexB->UV[w] = sprite->m_glCUV[i].y;
			vertexC->UV[z] = sprite->m_glCUV[i].z;
			vertexC->UV[w] = sprite->m_glCUV[i].w;
			vertexD->UV[z] = sprite->m_glCUV[i].x;
			vertexD->UV[w] = sprite->m_glCUV[i].w;
		}
	}
}